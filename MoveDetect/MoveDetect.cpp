#include "opencv.hpp"
#include "vector"

using namespace cv;

Mat MoveDetect(Mat last, Mat present);

int main(int argc, char **argv) 
{
	VideoCapture video;
	Mat Present, Last;
	video.open(0);
	video >> Last;
	waitKey(30);
	while (true)
	{
		video >> Present;
		//imshow("result", MoveDetect(last, present));
		MoveDetect(Last, Present);
		//imshow("present", present);
		//Present.copyTo(Last);
		video >> Last;
		waitKey(30);
	}
}

Mat MoveDetect(Mat last, Mat present)
{
	Mat grayPresent, diff, result;
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	cvtColor(last, last, COLOR_BGR2GRAY);
	cvtColor(present, grayPresent, COLOR_BGR2GRAY);
	absdiff(grayPresent, last, diff);
	threshold(diff, diff, 50, 255, CV_THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(diff, diff, element);
	dilate(diff, diff, element);
	result = diff.clone();
	Canny(diff, diff, 3, 9);
	findContours(diff, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	drawContours(present, contours, -1, Scalar(0, 0, 255));
	std::vector<Rect> rect(contours.size());
	//for (int i = 0; i < contours.size(); i++) 
	//{
	//	rect[i] = boundingRect(contours[i]);
	//	rectangle(present, rect[i], Scalar(0, 0, 255));
	//}
	//rectangle(present, boundingRect(contours), Scalar(0, 255, 0), 8);
	imshow("result", result);
	imshow("present", present);
	return result;
}