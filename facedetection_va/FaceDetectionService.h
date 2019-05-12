//
// Created by mdailey on 5/7/19.
//

#ifndef FACEDETECTION_VA_FACEDETECTIONSERVICE_H
#define FACEDETECTION_VA_FACEDETECTIONSERVICE_H

#include <webcc/rest_service.h>
#include <opencv2/objdetect/objdetect.hpp>


using namespace std;
using namespace cv;


class FaceDetectionService : public webcc::RestListService {
public:
    FaceDetectionService(int iSleepSeconds, CascadeClassifier &cascade);
protected:
    void Get(const webcc::UrlQuery& query, webcc::RestResponse* response) override;
    void Post(const std::string& request_content, webcc::RestResponse* response) override;
private:
    // Sleep some seconds before sending back the response for testing timeout control on client side.
    int _iSleepSeconds_;
    CascadeClassifier _cascade;
};


class Image
{
public:
    explicit Image() { }
    explicit Image(string faceId) { _faceId = faceId; }
    string getFaceId() { return _faceId; }
    void setImageData(string sImageDataBase64);
    bool faceDetect(CascadeClassifier &cascade, vector<Rect> &vFaces);
private:
    string _faceId;
    Mat _matInput;
};


#endif //FACEDETECTION_VA_FACEDETECTIONSERVICE_H
