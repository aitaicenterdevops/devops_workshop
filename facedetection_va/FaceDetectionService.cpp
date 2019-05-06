//
// Created by mdailey on 5/7/19.
//

#include <iostream>
#include <webcc/logger.h>
#include <webcc/rest_server.h>
#include <json/json.h>

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


Image *JsonToImage(const Json::Value &json)
{
    Image *pImage = new Image(json["faceId"].asString());
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


void FaceDetectionService::Post(const string &sRequestContent,
                                webcc::RestResponse* response)
{
    if (sRequestContent == "")
    {
        LOG_ERRO("Got empty request.");
    }
    Sleep(sleep_seconds_);
    Image *pImage = NULL;
    if (JsonStringToImage(sRequestContent, pImage))
    {
        // Execute workflow here
        string id = "new";

        Json::Value json;
        json["id"] = id;

        response->content = JsonToString(json);
        response->media_type = webcc::media_types::kApplicationJson;
        response->charset = "utf-8";
        response->status = webcc::Status::kCreated;
    } else {
        // Invalid JSON
        response->status = webcc::Status::kBadRequest;
    }
}


// Return empty list on GET /images for now

void FaceDetectionService::Get(const webcc::UrlQuery& query, webcc::RestResponse* response)
{
    Sleep(sleep_seconds_);
    Json::Value json(Json::arrayValue);
    response->content = JsonToString(json);
    response->media_type = webcc::media_types::kApplicationJson;
    response->charset = "utf-8";
    response->status = webcc::Status::kOK;
}

