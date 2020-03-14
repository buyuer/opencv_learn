#include "opencv2/opencv.hpp"
#include "Kinect.h"
#include "iostream"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	IKinectSensor *sensor = nullptr;
	GetDefaultKinectSensor(&sensor);
	sensor->Open();

	IDepthFrameSource *sourceDepth = nullptr;
	sensor->get_DepthFrameSource(&sourceDepth);

	int imageHeight = 0, imageWidth = 0;
	IFrameDescription *depthDescription = nullptr;
	sourceDepth->get_FrameDescription(&depthDescription);
	depthDescription->get_Height(&imageHeight);
	depthDescription->get_Width(&imageWidth);
	depthDescription->Release();

	IDepthFrameReader *readerDepth = nullptr;
	sourceDepth->OpenReader(&readerDepth);

	Mat tempImage(imageHeight, imageWidth, CV_16UC1);
	Mat image(imageHeight, imageWidth, CV_8UC1);

	IDepthFrame *frameDepth = nullptr;

	namedWindow("image");
	int fps = 0;
	while (true)
	{
		if (readerDepth->AcquireLatestFrame(&frameDepth) == S_OK)
		{
			frameDepth->CopyFrameDataToArray(imageHeight*imageWidth, (UINT16*)tempImage.data);
			tempImage.convertTo(image, CV_8UC1, 255.0 / 10000);
		
			imshow("image", image);
			frameDepth->Release();
			fps++;
		}
		if (waitKey(1) == 'q')
		{
			break;
		}
	}
	sourceDepth->Release();
	readerDepth->Release();
	sensor->Release();
	sensor->Close();
	cout << fps << endl;
	return 0;
}