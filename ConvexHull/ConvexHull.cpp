#include "opencv.hpp"
#include "vector"
using namespace cv;
using namespace std;
int main()
{
	Mat img(imread("2.jpg", 0));
	Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);
	Mat out1 = Mat::zeros(img.rows, img.cols, CV_8UC3);
	img = img > 119;
	vector<vector<Point>>contours;
	vector<Vec4i> hierarchy;
	Canny(img, img, 30, 90);
	findContours(img, contours, hierarchy, RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	int index = 0;
	for (; index >= 0; index = hierarchy[index][0])

	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(out, contours, index, color,	FILLED, 8, hierarchy);
	}
	vector<vector<Point>>  hull(contours.size());
	for (int i = 0; i < contours.size(); i++) 
	{
		convexHull(contours[i], hull[i], false);
	}
    drawContours(out1, hull, -1, Scalar(0, 152, 123));
	for (int i = 0; i < contours.size(); i++) 
	{
		RotatedRect box = minAreaRect((Mat)contours[i]);
		Point2f vertex[4], center;
		float radius = 0.0;
		box.points(vertex);
		for (int i = 0; i < 4; i++)
		{
			line(out1, vertex[i], vertex[(i + 1) % 4], Scalar(199, 322, 333), 2, LINE_AA);
			//cout << vertex[i];
		}
		minEnclosingCircle((Mat)contours[i], center, radius);
		circle(out, center, cvRound(radius), Scalar(1, 2, 164));
	}
	imshow("HULL", out1);
	imshow("contours", out);
	waitKey(0);

}