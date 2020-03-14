#include "opencv.hpp"
#include "vector"

using namespace std;
using namespace cv;
Mat g_CameraMatrix(3, 3, CV_32FC1, Scalar::all(0));
Mat g_DistCoeffs(1, 5, CV_32FC1, Scalar::all(0));
void Cross_Line_Detect(Mat image);
bool Line_Detect(vector<Point> array, int sumValue, int dis);
void Init_Camera(); 
int main()
{
	VideoCapture video;
	Mat image;
	Mat image1;
	video.open(0);
	Init_Camera();
	while (true)
	{
		video >> image;
		imshow("OrginalCamera", image);
		undistort(image, image1, g_CameraMatrix, g_DistCoeffs, g_CameraMatrix);
		imshow("Camera", image1);
		
		Cross_Line_Detect(image1);
		waitKey(30);
	}
}
void Cross_Line_Detect(Mat image) 
{
	Mat draw = Mat(image.rows, image.cols, CV_8UC3, Scalar::all(0));
	vector<Mat> channelsHSV(3);
	vector<vector<Point>> contours;
	vector<vector<Point>> lineContours;
	cvtColor(image, image, COLOR_BGR2HSV);
	split(image, channelsHSV);
	image = channelsHSV[0];
	threshold(image, image, 100, 255, 0);
	blur(image, image, Size(3, 3));
	Canny(image, image, 150, 450);
	findContours(image, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		if (Line_Detect(contours[i], 500, 2))
		{
			lineContours.push_back(contours[i]);
		}
	}
	drawContours(draw, lineContours, -1, Scalar(255, 255, 255));
	imshow("draw", draw);
}

bool Line_Detect(vector<Point> array, int sumValue, int dis)
{
	int count = array.size() - 1;
	int x, y, sum;
	sum = 0;
	for (int i = 0; i < count; i++)
	{
		x = abs(array[i].x - array[i + 1].x);
		y = abs(array[i].y - array[i + 1].y);
		if (x < dis && y < dis)
		{
			sum++;
		}
	}
	if (sum > sumValue)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Init_Camera()
{
	g_CameraMatrix.at<float>(0, 0) = 532.3586852140176F;
	g_CameraMatrix.at<float>(0, 1) = 0.0F;
	g_CameraMatrix.at<float>(0, 2) = 316.8296169461942F;
	g_CameraMatrix.at<float>(1, 0) = 0.0F;
	g_CameraMatrix.at<float>(1, 1) = 530.6144435519142F;
	g_CameraMatrix.at<float>(1, 2) = 255.899227881748F;
	g_CameraMatrix.at<float>(2, 0) = 0.0F;
	g_CameraMatrix.at<float>(2, 1) = 0.0F;
	g_CameraMatrix.at<float>(2, 2) = 1.0F;
	g_DistCoeffs.at<float>(0, 0) = -0.4406166337986711F;
	g_DistCoeffs.at<float>(0, 1) = 0.2829849800313692F;
	g_DistCoeffs.at<float>(0, 2) = -0.00194932968590072F;
	g_DistCoeffs.at<float>(0, 3) = -0.001100026054179157F;
	g_DistCoeffs.at<float>(0, 4) = -0.1302761200772676F;
	cout << g_CameraMatrix << endl;
	cout << g_DistCoeffs << endl;
}