#include "opencv.hpp"
#include "vector"
using namespace cv;
using namespace std;
bool Line_Detect(vector<Point> array, int sum,int dis);
int main(int argc, char **argv) 
{
	Mat image(imread("4.jpg")), draw, draw1, draw2;
	vector<Mat> channelsHSV(3), channelsBGR(3);
	imshow("image", image);
	split(image, channelsBGR);
	cvtColor(image, image, COLOR_BGR2HSV);
	split(image, channelsHSV);
	imshow("ch0HSV", channelsHSV[0]);
	imshow("ch1HSV", channelsHSV[1]);
	imshow("ch2HSV", channelsHSV[2]);
	imshow("ch0BRG", channelsBGR[0]);
	imshow("ch1BGR", channelsBGR[1]);
	imshow("ch2BGR", channelsBGR[2]);
	/*cvtColor(image, image, COLOR_HSV2BGR);
	cvtColor(image, image, COLOR_BGR2GRAY);*/
	image = channelsHSV[0];
	draw = Mat(image.rows, image.cols, CV_8UC3, Scalar::all(0));
	draw1 = Mat(image.rows, image.cols, CV_8UC3, Scalar::all(0));
	draw2 = Mat(image.rows, image.cols, CV_8UC3, Scalar::all(0));
	threshold(image, image,100, 255, 0);
	imshow("threshold", image);
	blur(image, image, Size(3,3 ));
	imshow("Blur", image);
	Canny(image, image, 150, 450);
	imshow("Canny", image);
	//cout << image.type() << endl;
	vector<vector<Point>> contours;
	findContours(image, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	imshow("draw", draw);
	for (int i = 0; i < contours.size(); i++) 
	{
		//waitKey();
		drawContours(draw, contours, i, Scalar(255, 255, 255));
		cout << contourArea(contours[i], 0) << endl;
		imshow("draw", draw);
	}
	vector<vector<Point>> realContours;
	/*for (int i = 0; i < contours.size(); i++) 
	{
		if (contourArea(contours[i])>=10)
		{
			realContours.push_back(contours[i]);
		}
	}
	drawContours(draw1, realContours, -1, Scalar(0, 0, 255));*/
	//drawContours(draw, contours, -1, Scalar(0, 0, 255));
	//boundingRect(contours[0]);
	//rectangle(draw, boundingRect(contours[0]), Scalar(0, 255, 0));
	vector<vector<Point>> lineContours;
	for (int i = 0; i < contours.size(); i++) 
	{
		if (Line_Detect(contours[i], 500, 2))
		{
			lineContours.push_back(contours[i]);
		}
	}
	for (int i=0; i < lineContours.size(); i++) 
	{
		waitKey(1000);
		drawContours(draw2, lineContours, i, Scalar(255, 255, 255));
		imshow("draw2", draw2);
	}
	cout << "lineContours£»" << lineContours.size() << endl;
	for (int i = 0; i < lineContours[0].size(); i++) 
	{
		cout << lineContours[0][i] << endl;
	}
	cout << draw2.size() << endl;
	imshow("draw2", draw2);
	imshow("draw", draw);
	//imshow("draw1", draw1);
	cout << contours.size() << endl;
	waitKey();
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