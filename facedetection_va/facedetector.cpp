#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "DB.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <webcc/logger.h>
#include <webcc/rest_server.h>
#include <json/json.h>

using namespace std;
using namespace cv;

static void Sleep(int seconds) {
	if (seconds > 0) {
		LOG_INFO("Sleep %d seconds...", seconds);
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
	}
}

class FaceDetectionService : public webcc::RestListService {
public:
    explicit FaceDetectionService(int sleep_seconds)
            : sleep_seconds_(sleep_seconds) { }
protected:
	void Get(const webcc::UrlQuery& query, webcc::RestResponse* response) override;
    void Post(const std::string& request_content, webcc::RestResponse* response) override;
private:
    // Sleep some seconds before send back the response.
    // For testing timeout control in client side.
    int sleep_seconds_;
};


void FaceDetectionService::Post(const std::string& request_content,
                                webcc::RestResponse* response)
{

}


void FaceDetectionService::Get(const webcc::UrlQuery& query, webcc::RestResponse* response)
{
	Sleep(sleep_seconds_);

	Json::Value json(Json::arrayValue);
    json.append("{\"faceId\":\"101\"");

	// TODO: Simplify
	response->content = JsonToString(json);
	response->media_type = webcc::media_types::kApplicationJson;
	response->charset = "utf-8";
	response->status = webcc::Status::kOK;
}


int main(int argc, char *argv[])
{
    //string sFixedPath = "/home/jednipat/works/devops_workshop/facedetection_webapp/public" ;
	//string sFilePath = string(argv[1]); // Example path: /uploads/fimage64/image/21/image.png
	//string sFaceId = string(argv[2]);   // The face ID stored in the database
	//string sFullPath = sFixedPath + sFilePath;
	//string sFaceFilePath = sFixedPath + sFilePath.substr(0, sFilePath.find("image.png")) + "face.jpg";
	//std::cout << "sFaceFilePath: " << sFaceFilePath << std::endl;

	string sFaceId = "101";
    WEBCC_LOG_INIT("", webcc::LOG_CONSOLE);
    uint16_t port = 444;
    size_t workers = 2;
    int sleep_seconds = 0;

    try {
        webcc::RestServer server(port, workers);
        server.Bind(std::make_shared<FaceDetectionService>(sleep_seconds), "/images", false);
        server.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

	DB *pDB = nullptr;
    //DB *pDB = new DB();

	CascadeClassifier face_cascade;
	face_cascade.load( "/home/jednipat/works/devops_workshop/facedetection_va/haarcascade_frontalface_alt.xml" ) ;  
	cv::Mat mInput;
	//mInput = cv::imread(sFullPath, 1);

	return 0;

	std::vector<Rect> faces;
	cv::Mat mInputGray;

    cvtColor( mInput, mInputGray, CV_BGR2GRAY );
  	equalizeHist( mInputGray, mInputGray );

  	face_cascade.detectMultiScale( mInputGray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

	for(int i=0; i < faces.size(); i++)
	{
		cv::Mat mCroppedFace;
		mInput(faces.at(i)).copyTo(mCroppedFace);
		//cv::imwrite(sFaceFilePath,mCroppedFace);

		stringstream ssFaceWidth;
		stringstream ssFaceHeight;
		ssFaceWidth << mCroppedFace.cols;
		ssFaceHeight << mCroppedFace.rows;
		pDB->updateFaceInfo(sFaceId, "true", ssFaceWidth.str() , ssFaceHeight.str());

		mCroppedFace.release();
	}

	std::cout << faces.size() << std::endl;

	return 1;
}
