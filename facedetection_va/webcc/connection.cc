#include "webcc/connection.h"

#include <utility>  // for move()

#include "boost/asio/write.hpp"

#include "webcc/connection_pool.h"
#include "webcc/logger.h"
#include "webcc/request_handler.h"

using boost::asio::ip::tcp;

namespace webcc {

Connection::Connection(tcp::socket socket, ConnectionPool* pool,
                       RequestHandler* handler)
    : socket_(std::move(socket)),
      pool_(pool),
      buffer_(kBufferSize),
      request_handler_(handler) {
}

void Connection::Start() {
  request_.reset(new Request{});
  request_parser_.Init(request_.get());
  DoRead();
}

void Connection::Close() {
  LOG_INFO("Close socket...");

  boost::system::error_code ec;
  socket_.close(ec);
  if (ec) {
    LOG_ERRO("Socket close error (%s).", ec.message().c_str());
  }
}

void Connection::SendResponse(ResponsePtr response) {
  assert(response);

  response_ = response;

  if (request_->IsConnectionKeepAlive()) {
    response_->SetHeader(headers::kConnection, "Keep-Alive");
  } else {
    response_->SetHeader(headers::kConnection, "Close");
  }

  response_->Prepare();

  DoWrite();
}

void Connection::SendResponse(Status status) {
  SendResponse(std::make_shared<Response>(status));
}

void Connection::DoRead() {
  socket_.async_read_some(boost::asio::buffer(buffer_),
                          std::bind(&Connection::OnRead, shared_from_this(),
                                    std::placeholders::_1,
                                    std::placeholders::_2));
}

void Connection::OnRead(boost::system::error_code ec, std::size_t length) {
  if (ec) {
    LOG_ERRO("Socket read error (%s).", ec.message().c_str());
    if (ec != boost::asio::error::operation_aborted) {
      pool_->Close(shared_from_this());
    }
    return;
  }

  if (!request_parser_.Parse(buffer_.data(), length)) {
    // Bad request.
    LOG_ERRO("Failed to parse HTTP request.");
    SendResponse(Status::kBadRequest);
    return;
  }

  if (!request_parser_.finished()) {
    // Continue to read the request.
    DoRead();
    return;
  }

  LOG_VERB("HTTP request:\n%s", request_->Dump(4, "> ").c_str());

  // Enqueue this connection.
  // Some worker thread will handle it later.
  request_handler_->Enqueue(shared_from_this());
}

void Connection::DoWrite() {
  LOG_VERB("HTTP response:\n%s", response_->Dump(4, "> ").c_str());

  boost::asio::async_write(socket_, response_->payload(),
                           std::bind(&Connection::OnWrite, shared_from_this(),
                                     std::placeholders::_1,
                                     std::placeholders::_2));
}

// NOTE:
// This write handler will be called from main thread (the thread calling
// io_context.run), even though AsyncWrite() is invoked by worker threads.
// This is ensured by Asio.
void Connection::OnWrite(boost::system::error_code ec, std::size_t length) {
  if (ec) {
    LOG_ERRO("Socket write error (%s).", ec.message().c_str());

    if (ec != boost::asio::error::operation_aborted) {
      pool_->Close(shared_from_this());
    }
  } else {
    LOG_INFO("Response has been sent back, length: %u.", length);

    if (request_->IsConnectionKeepAlive()) {
      LOG_INFO("The client asked for a keep-alive connection.");
      LOG_INFO("Continue to read the next request...");
      Start();
    } else {
      Shutdown();
      pool_->Close(shared_from_this());
    }
  }
}

// Socket close VS. shutdown:
//   https://stackoverflow.com/questions/4160347/close-vs-shutdown-socket
void Connection::Shutdown() {
  LOG_INFO("Shutdown socket...");

  // Initiate graceful connection closure.
  boost::system::error_code ec;
  socket_.shutdown(tcp::socket::shutdown_both, ec);

  if (ec) {
    LOG_ERRO("Socket shutdown error (%s).", ec.message().c_str());
  }
}

}  // namespace webcc
