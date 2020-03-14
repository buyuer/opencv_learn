#include "iostream"
#include "vector"
#include "opencv.hpp"
using namespace std;
using namespace cv;

void Fit_Line(vector<Point2d> points);
Mat drawImage(500, 500, CV_8UC3);
int main(int argc,char** argv) 
{
	RNG random(time(0));
	vector<Point2d> point;
	for (int i = 0; i < 100; i++) 
	{
		Point2f ranPoint(random.uniform(5*i, 5*i+10), random.uniform(5*i, 5*i+10));
		point.push_back(ranPoint);
		cout << ranPoint << endl;
		circle(drawImage, ranPoint, 2, Scalar(0, 0, 255), -1);
	}
	for (int i = 0; i < 10; i++) 
	{
		Point2f ranPoint(random.uniform(0,500), random.uniform(0,500));
		point.push_back(ranPoint);
		cout << ranPoint << endl;
		circle(drawImage, ranPoint, 2, Scalar(0, 0, 255), -1);
	}
	Fit_Line(point);
	imshow("drawImage", drawImage);
	waitKey();
}

void Fit_Line(vector<Point2d> points) 
{
	int pointNum = points.size();
	Point2f point1, point2;
	double x_ = 0, y_ = 0;
	double a = 0, b = 0;
	double b_zi = 0, b_mu = 0;
	int i;
	for (i = 0; i < pointNum; i++)
	{
		x_ += points[i].x;
		y_ += points[i].y;
	}
	x_ /= pointNum;
	y_ /= pointNum;
	for (i = 0; i < pointNum; i++) 
	{
		b_zi += (points[i].x - x_)*(points[i].y - y_);
		b_mu += (points[i].x - x_)*(points[i].x - x_);
	}
	b = b_zi / b_mu;
	a = y_ - b*x_;
	point1.x = 0;
	point1.y = b*point1.x + a;
	point2.x = 500;
	point2.y = b*point2.x + a;
	line(drawImage, point1, point2, Scalar(255, 0, 0));
	cout << "a:" << a << endl << "b: " << b << endl;
}