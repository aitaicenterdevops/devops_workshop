#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <cstring>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	string sFixedPath = "/home/jednipat/works/devops_workshop/facedetection_webapp/public";
	string sFilePath = string(argv[1]); // Example path: /uploads/fimage64/image/21/image.png
	string sFullPath = sFixedPath + sFilePath;
	string sFaceFilePath = sFixedPath + sFilePath.substr(0, sFilePath.find("image.png")) + "face.jpg";
	std::cout << "sFaceFilePath: " << sFaceFilePath << std::endl;

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
		mCroppedFace.release();
	}

	std::cout << faces.size() << std::endl;

	return 1;
}
