#include "opencv2/opencv.hpp"
#include "vector"

#include "argument.h"

#define mat Mat

using namespace cv;
using namespace std;

Point2d centre;
mat frame,inputImage;
Mat g_CameraMatrix(3, 3, CV_32FC1, Scalar::all(0));
Mat g_DistCoeffs(1, 5, CV_32FC1, Scalar::all(0));

void camera();
void Detect(mat &inputImage);
void Find_CentreSpot(mat &inputImage, Point2d &centre);
void Get_XY(double &PX, double &PY, double &X, double &Y);
void Camera_Init();
int main(int argc,char** argv)
{
	camera();
	return 0;
}

void camera() 
{
	VideoCapture camera;
	Camera_Init();
	//camera.open(1);
	camera.open("2.avi");
	while (camera.grab())
	{
		camera >> inputImage;
		undistort(inputImage, frame, g_CameraMatrix, g_DistCoeffs, g_CameraMatrix);
		Detect(frame);
		if(waitKey(1)=='q')
		{
			break;
		}
	}
}

void Detect(mat &inputImage) 
{
	vector<mat> hsvChannels(3);

	split(inputImage, hsvChannels);
	Mat openKernel = getStructuringElement(MORPH_RECT, Size(10, 10));
	Mat dilateKernel = getStructuringElement(MORPH_RECT, Size(7, 7));
	Mat erodeKernel = getStructuringElement(MORPH_RECT, Size(7, 7));

	mat tempImage = hsvChannels[0];
	for (int i = 0; i < 3; i++)
	{
		blur(tempImage, tempImage, Size(3, 3));
	}
	for (int i = 0; i < 2; i++)
	{
		morphologyEx(tempImage, tempImage, MORPH_OPEN, openKernel);
	}
	for (int i = 0; i < 5; i++)
	{
		blur(tempImage, tempImage, Size(3, 3));
	}
	mat dilateImage,erodeImage;
	tempImage.copyTo(dilateImage);
	tempImage.copyTo(erodeImage);
	dilate(dilateImage, dilateImage, dilateKernel);
	erode(erodeImage, erodeImage, erodeKernel);
	mat gradientImage = dilateImage - erodeImage;
	//threshold(gradientImage, gradientImage, 50, 255, THRESH_BINARY);
    imshow("hsv-H", gradientImage);

	Find_CentreSpot(gradientImage, centre);
}

void Detect2(mat &inputImage) 
{
	vector<mat> hsvChannels(3);

	split(inputImage, hsvChannels);
	Mat openKernel = getStructuringElement(MORPH_RECT, Size(10, 10));
	Mat closeKernel = getStructuringElement(MORPH_RECT, Size(10, 10));
	
	mat tempImage = hsvChannels[0];
	for (int i = 0; i < 1; i++) 
	{
		blur(tempImage, tempImage, Size(3, 3));
	}
	for (int i = 0; i < 2; i++) 
	{
		morphologyEx(tempImage, tempImage, MORPH_OPEN, openKernel);
	}
	for (int i = 0; i < 0; i++)
	{
		blur(tempImage, tempImage, Size(3, 3));
	}
	for (int i = 0; i < 2; i++)
	{
		morphologyEx(tempImage, tempImage, MORPH_CLOSE, closeKernel);
	}

	imshow("hsv-H", tempImage);

	Find_CentreSpot(tempImage,centre);
}

void Find_CentreSpot(mat &inputImage, Point2d &centre) 
{
	vector<Point2d> corners;
	vector<Point2d> realCorners(0);

	goodFeaturesToTrack(inputImage, corners, 7, 0.19, 30);

	for (unsigned int i = 0; i<corners.size(); i++)
	{
		if (corners[i].x>20 && corners[i].y>20 && corners[i].x<620 && corners[i].y<460)
		{
			realCorners.push_back(corners[i]);
		}
	}
	
	centre.x = 0;
	centre.y = 0;
	
	for (unsigned int i = 0; i < realCorners.size(); i++)
	{
		circle(frame, realCorners[i], 5, Scalar(0, 0, 255));
		centre.x += realCorners[i].x;
		centre.y += realCorners[i].y;
	}

	centre.x /= realCorners.size();
	centre.y /= realCorners.size();

	circle(frame, centre, 5, Scalar(50, 2, 200), -1);
	cout << frame.size();
	cout << centre << endl;
	double X, Y;
	Get_XY(centre.x, centre.y, X, Y);
	
	cout << endl;
	cout << "X: " << X << endl;
	cout << "Y: " << Y << endl;
	cout << endl;
	
	imshow("camera", frame);
}

void Find_CentreSpot1(mat &inputImage, Point2d &centre) 
{
	vector<Point2d> corners;

	goodFeaturesToTrack(inputImage, corners, 12, 0.1, 25);

	for (unsigned int i = 0; i < corners.size(); i++)
	{
		circle(frame, corners[i], 5, Scalar(0, 0, 255));
	}
	
	imshow("camera", frame);

}
void Get_XY(double &PX, double &PY, double &X, double &Y) 
{
	double IX(PX - CX);
	double IY(PY - CY);
	double WX((HEIGHT*IX) / FX), WY((HEIGHT*IY) / FY);
	X = WX;
	Y = WY;
}
void Get_XY1(double &PX, double &PY, double &X, double &Y)
{
	double IX(PX - CX);
	double IY(PY - CY);
	double WX(0), WY(0);
	if (IY >= 0)
	{
		//WY = HEIGHT * tan((CV_PI / 2) - OVERLOOK_ANGLE - atan(abs(IY) / FY));
		WY = HEIGHT * tan(OVERLOOK_ANGLE + atan(abs(IY) / FY));
	}
	else 
	{
		WY = HEIGHT / tan(OVERLOOK_ANGLE - atan(abs(IY) / FY));
	}
	WX = ((HEIGHT / sin(OVERLOOK_ANGLE)) / FX) * IX;
	if (WX < 0) WX = -WX;
	//X = WX * cos(ROTATE_ANGLE) + WY * sin(ROTATE_ANGLE);
	//Y = WY * cos(ROTATE_ANGLE) - WX * sin(ROTATE_ANGLE);
	X = WX;
	Y = WY;
}

void Camera_Init() 
{
	g_CameraMatrix.at<float>(0, 0) = 459.6343093689025F;
	g_CameraMatrix.at<float>(0, 1) = 0.0F;
	g_CameraMatrix.at<float>(0, 2) = 295.346146230927F;
	g_CameraMatrix.at<float>(1, 0) = 0.0F;
	g_CameraMatrix.at<float>(1, 1) = 459.5553265929037F;
	g_CameraMatrix.at<float>(1, 2) = 227.3759827303813F;
	g_CameraMatrix.at<float>(2, 0) = 0.0F;
	g_CameraMatrix.at<float>(2, 1) = 0.0F;
	g_CameraMatrix.at<float>(2, 2) = 1.0F;
	g_DistCoeffs.at<float>(0, 0) = -0.05843058134963959F;
	g_DistCoeffs.at<float>(0, 1) = 0.2525352062398861F;
	g_DistCoeffs.at<float>(0, 2) = -0.001888432427034808F;
	g_DistCoeffs.at<float>(0, 3) = -0.002962239528505254F;
	g_DistCoeffs.at<float>(0, 4) = -0.2865184929689795F;
}

