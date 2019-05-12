//
// Created by mdailey on 5/7/19.
//

#include <iostream>
#include <webcc/logger.h>
#include <webcc/rest_server.h>
#include <json/json.h>
#include <base64/base64.h>
#include <opencv2/imgcodecs.hpp>
#include <cv.hpp>

#include "FaceDetectionService.h"

FaceDetectionService::FaceDetectionService(int iSleepSeconds, CascadeClassifier &cascade)
        : _iSleepSeconds_(iSleepSeconds), _cascade(cascade)
{
    if (_cascade.empty())
    {
        LOG_ERRO("Cascade is empty!");
    }
}


static void Sleep(int seconds) {
    if (seconds > 0) {
        LOG_INFO("Sleep %d seconds...", seconds);
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }
}


Json::Value StringToJson(const string &s)
{
    Json::Value json;

    Json::CharReaderBuilder builder;
    std::stringstream stream(s);
    std::string errs;
    if (!Json::parseFromStream(builder, stream, &json, &errs)) {
        LOG_ERRO(errs.c_str());
    }
    return json;
}


void Image::setImageData(string sImageDataBase64)
{
    string sImageData = base64_decode(sImageDataBase64);
    if (sImageData == "")
    {
        return;
    }
    vector<uint8_t> vImageData(sImageData.begin(), sImageData.end());
    _matInput = imdecode(vImageData, IMREAD_GRAYSCALE);
}


Image *JsonToImage(const Json::Value &json)
{
    if (json["faceId"].empty() || json["imageData"].empty())
    {
        return nullptr;
    }
    Image *pImage = new Image(json["faceId"].asString());
    if (!json["imageData"].isNull())
    {
        pImage->setImageData(json["imageData"].asString());
    }
    return pImage;
}


bool JsonStringToImage(const std::string &sJson, Image *&pImage)
{
    Json::Value json = StringToJson(sJson);
    if (json.empty())
    {
        return false;
    }
    pImage = JsonToImage(json);
    if (pImage == nullptr)
    {
        return false;
    }
    else
    {
        return true;
    }
}


string JsonToString(const Json::Value &json)
{
    Json::StreamWriterBuilder builder;
    return Json::writeString(builder, json);
}


bool Image::faceDetect(CascadeClassifier &cascade, vector<Rect> &vFaces)
{
    if (_matInput.empty())
    {
        return false;
    }
    equalizeHist(_matInput, _matInput);
    cascade.detectMultiScale(_matInput, vFaces, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30));
    return true;
}


void FaceDetectionService::Post(const string &sRequestContent,
                                webcc::RestResponse* response)
{
    Sleep(_iSleepSeconds_);
    if (_cascade.empty())
    {
        // Cascade not created/initialized
        LOG_ERRO("Face detection cascade is invalid.");
        response->status = webcc::Status::kInternalServerError;
        return;
    }

    response->status = webcc::Status::kBadRequest;

    if (sRequestContent == "")
    {
        // Empty POST data
        LOG_ERRO("Got empty request.");
        return;
    }

    Image *pImage = nullptr;
    if (!JsonStringToImage(sRequestContent, pImage))
    {
        // Invalid JSON
        LOG_ERRO("Could not extract image from POST data.");
        if (pImage) delete pImage;
        return;
    }

    // Create JSON response

    Json::Value json;
    json["faceId"] = pImage->getFaceId();
    json["faces"] = Json::Value(Json::arrayValue);

    vector<Rect> vFaces;
    if (!pImage->faceDetect(_cascade, vFaces))
    {
        // Invalid image data
        LOG_ERRO("Image data invalid.");
        delete pImage;
        return;
    }

    for (int i=0; i < (int)vFaces.size(); i++)
    {
        Json::Value jsonFace;
        jsonFace["width"] = vFaces[i].width;
        jsonFace["height"] = vFaces[i].height;
        jsonFace["x"] = vFaces[i].x;
        jsonFace["y"] = vFaces[i].y;
        json["faces"].append(jsonFace);
    }

    response->content = JsonToString(json);
    response->media_type = webcc::media_types::kApplicationJson;
    response->charset = "utf-8";
    response->status = webcc::Status::kCreated;

    delete pImage;
}


// Return empty list on GET /images for now

void FaceDetectionService::Get(const webcc::UrlQuery& query, webcc::RestResponse* response)
{
    Sleep(_iSleepSeconds_);
    Json::Value json(Json::arrayValue);
    response->content = JsonToString(json);
    response->media_type = webcc::media_types::kApplicationJson;
    response->charset = "utf-8";
    response->status = webcc::Status::kOK;
}

