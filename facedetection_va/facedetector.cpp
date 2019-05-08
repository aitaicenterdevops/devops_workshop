
#include <iostream>
#include <cstring>
#include <vector>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <webcc/logger.h>
#include <webcc/rest_server.h>

#include "DB.h"
#include "FaceDetectionService.h"


using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    if (argc > 0 && string(argv[1]) == "server")
    {
        WEBCC_LOG_INIT("", webcc::LOG_CONSOLE);
        uint16_t port = 444;
        size_t workers = 2;
        int sleep_seconds = 0;

        CascadeClassifier face_cascade;
        face_cascade.load("/home/dev/devops_workshop/facedetection_va/haarcascade_frontalface_alt.xml");
        if (face_cascade.empty())
        {
            LOG_ERRO("Cascade is empty!");
            return 1;
        }

        try {
            webcc::RestServer server(port, workers);
            server.Bind(std::make_shared<FaceDetectionService>(sleep_seconds, face_cascade), "/images", false);
            server.Run();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
            return 1;
        }
    }
    else
    {
        string sFixedPath = "/home/jednipat/works/devops_workshop/facedetection_webapp/public" ;
        string sFilePath = string(argv[1]); // Example path: /uploads/fimage64/image/21/image.png
        string sFaceId = string(argv[2]);   // The face ID stored in the database
        string sFullPath = sFixedPath + sFilePath;
        string sFaceFilePath = sFixedPath + sFilePath.substr(0, sFilePath.find("image.png")) + "face.jpg";
        std::cout << "sFaceFilePath: " << sFaceFilePath << std::endl;

        DB *pDB = new DB();

        CascadeClassifier face_cascade;
        face_cascade.load( "/home/jednipat/works/devops_workshop/facedetection_va/haarcascade_frontalface_alt.xml" ) ;
        cv::Mat mInput;
        mInput = cv::imread(sFullPath, 1);

        std::vector<Rect> faces;
        cv::Mat mInputGray;

        cvtColor( mInput, mInputGray, CV_BGR2GRAY );
        equalizeHist( mInputGray, mInputGray );

        face_cascade.detectMultiScale( mInputGray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

        for(int i=0; i < faces.size(); i++)
        {
            cv::Mat mCroppedFace;
            mInput(faces.at(i)).copyTo(mCroppedFace);
            cv::imwrite(sFaceFilePath,mCroppedFace);

            stringstream ssFaceWidth;
            stringstream ssFaceHeight;
            ssFaceWidth << mCroppedFace.cols;
            ssFaceHeight << mCroppedFace.rows;
            pDB->updateFaceInfo(sFaceId, "true", ssFaceWidth.str() , ssFaceHeight.str());

            mCroppedFace.release();
        }

        std::cout << faces.size() << std::endl;
    }
	return 0;
}
