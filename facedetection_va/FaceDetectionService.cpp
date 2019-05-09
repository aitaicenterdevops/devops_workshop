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
        std::cerr << errs << std::endl;
    }
    return json;
}


void Image::setImageData(string sImageDataBase64)
{
    string sImageData = base64_decode(sImageDataBase64);
    vector<uint8_t> vImageData(sImageData.begin(), sImageData.end());
    _matInput = imdecode(vImageData, IMREAD_GRAYSCALE);
}


Image *JsonToImage(const Json::Value &json)
{
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
    if (!json)
    {
        return false;
    }
    pImage = JsonToImage(json);
    return true;
}


string JsonToString(const Json::Value &json)
{
    Json::StreamWriterBuilder builder;
    return Json::writeString(builder, json);
}


void Image::faceDetect(CascadeClassifier &cascade, vector<Rect> &vFaces)
{
    if (_matInput.empty())
    {
        LOG_ERRO("Input image is empty!");
        return;
    }
    equalizeHist(_matInput, _matInput);
    cascade.detectMultiScale(_matInput, vFaces, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30));
}


void FaceDetectionService::Post(const string &sRequestContent,
                                webcc::RestResponse* response)
{
    if (sRequestContent == "")
    {
        LOG_ERRO("Got empty request.");
        return;
    }
    Sleep(_iSleepSeconds_);
    Image *pImage = nullptr;
    if (JsonStringToImage(sRequestContent, pImage))
    {
        Json::Value json;

        json["faceId"] = pImage->getFaceId();
        json["faces"] = Json::Value(Json::arrayValue);

        vector<Rect> vFaces;
        pImage->faceDetect(_cascade, vFaces);

        for (int i=0; i < vFaces.size(); i++)
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
    } else {
        // Invalid JSON
        response->status = webcc::Status::kBadRequest;
    }
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

