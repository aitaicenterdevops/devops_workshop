//
// Created by mdailey on 5/7/19.
//

#ifndef FACEDETECTION_VA_FACEDETECTIONSERVICE_H
#define FACEDETECTION_VA_FACEDETECTIONSERVICE_H

#include <webcc/rest_service.h>

using namespace std;


class FaceDetectionService : public webcc::RestListService {
public:
    explicit FaceDetectionService(int sleep_seconds)
            : sleep_seconds_(sleep_seconds) { }
protected:
    void Get(const webcc::UrlQuery& query, webcc::RestResponse* response) override;
    void Post(const std::string& request_content, webcc::RestResponse* response) override;
private:
    // Sleep some seconds before sending back the response.
    // For testing timeout control in client side.
    int sleep_seconds_;
};


class Image
{
public:
    explicit Image() { _faceId = nullptr; }
    explicit Image(string faceId) { _faceId = faceId; }
    string getFaceId() { return _faceId; }
private:
    string _faceId;
};


#endif //FACEDETECTION_VA_FACEDETECTIONSERVICE_H
