#ifndef WEBCC_PARSER_H_
#define WEBCC_PARSER_H_

#include <string>

#include "webcc/common.h"
#include "webcc/globals.h"

namespace webcc {

class Message;

// HTTP request and response parser.
class Parser {
public:
  explicit Parser(Message* message);

  virtual ~Parser() = default;

  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;

  void Init(Message* message);

  bool finished() const { return finished_; }

  std::size_t content_length() const { return content_length_; }

  bool Parse(const char* data, std::size_t length);

protected:
  // Reset for next parse.
  void Reset();

  // Parse headers from pending data.
  // Return false only on syntax errors.
  bool ParseHeaders();

  // Get next line (using delimiter CRLF) from the pending data.
  // The line will not contain a trailing CRLF.
  // If |erase| is true, the line, as well as the trailing CRLF, will be erased
  // from the pending data.
  bool GetNextLine(std::size_t off, std::string* line, bool erase);

  virtual bool ParseStartLine(const std::string& line) = 0;

  bool ParseHeaderLine(const std::string& line);

  virtual bool ParseContent(const char* data, std::size_t length);

  bool ParseFixedContent(const char* data, std::size_t length);

  bool ParseChunkedContent(const char* data, std::size_t length);
  bool ParseChunkSize();

  // Return false if the compressed content cannot be decompressed.
  bool Finish();

  void AppendContent(const char* data, std::size_t count);
  void AppendContent(const std::string& data);

  // TODO: Rename to IsFixedContentFull.
  bool IsContentFull() const;

  // Check header Content-Encoding to see if the content is compressed.
  bool IsContentCompressed() const;

protected:
  // The result HTTP message.
  Message* message_;

  // Data waiting to be parsed.
  std::string pending_data_;

  // Temporary data and helper flags for parsing.
  std::size_t content_length_;
  std::string content_;
  bool start_line_parsed_;
  bool content_length_parsed_;
  bool header_ended_;
  bool chunked_;
  std::size_t chunk_size_;
  bool finished_;
};

}  // namespace webcc

#endif  // WEBCC_PARSER_H_
