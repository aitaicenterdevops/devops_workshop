
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <webcc/logger.h>

#include "FaceDetectionService.h"

using ::testing::MatchesRegex;
using ::testing::DoAll;
using ::testing::InvokeWithoutArgs;
using ::testing::Return;
using ::testing::_;

// Make subclass of FaceDetectionService so we can test its protected methods

class TestFaceDetectionService : public FaceDetectionService {
public:
    explicit TestFaceDetectionService(int iSleepSeconds, CascadeClassifier &cascade)
            : FaceDetectionService(iSleepSeconds, cascade) { }
public:
    void Get(const webcc::UrlQuery& query, webcc::RestResponse* response) { FaceDetectionService::Get(query, response); }
    void Post(const std::string& request_content, webcc::RestResponse* response) { FaceDetectionService::Post(request_content, response); }
};

// Test GET response

TEST(FaceDetectionService, Get)
{
    CascadeClassifier cascadeClassifier;
    auto pFaceDetectionService = new TestFaceDetectionService(0, cascadeClassifier);
    ASSERT_NE(pFaceDetectionService, nullptr);

    webcc::UrlQuery query;
    webcc::RestResponse response;
    pFaceDetectionService->Get(query, &response);

    ASSERT_EQ(response.status, webcc::Status::kOK);
}

// Test POST response

TEST(FaceDetectionService, Post)
{
    CascadeClassifier cascadeClassifier;
    auto pFaceDetectionService = new TestFaceDetectionService(0, cascadeClassifier);
    ASSERT_NE(pFaceDetectionService, nullptr);

    // Empty request generates error log

    WEBCC_LOG_INIT("", webcc::LOG_CONSOLE);

    std::string sRequestContent = "";
    webcc::RestResponse response;
    testing::internal::CaptureStderr();
    pFaceDetectionService->Post(sRequestContent, &response);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_THAT(output, MatchesRegex(".*Got empty request.*"));

    // Need more tests here!
}
