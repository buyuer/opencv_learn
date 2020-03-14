#include "iostream"
#include "vector"

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

/*green*/
#define greenLowScalar Scalar(38, 125, 50)
#define greenHighScalar Scalar(75, 255, 255)
/*green*/

void scan_row(Mat&inputImage, vector<Point>&out);
void scan_col(Mat&inputImage, vector<Point>&out);

int main() 
{
	VideoCapture camera(1);
	Mat frame, hsvImage, binImage;
	Mat greenBinImage;
	Mat hsvChannel[3];

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));

	int lowH = 111, lowS = 111, lowV = 111;
	int highH = 130, highS = 130, highV = 130;

	namedWindow("adjust");
	createTrackbar("lowH", "adjust", &lowH, 179);
	createTrackbar("lowS", "adjust", &lowS, 255);
	createTrackbar("lowV", "adjust", &lowV, 255);
	createTrackbar("highH", "adjust", &highH, 179);
	createTrackbar("highS", "adjust", &highS, 255);
	createTrackbar("highV", "adjust", &highV, 255);

	while (camera.grab()) 
	{
		camera >> frame;
		cvtColor(frame, hsvImage, COLOR_BGR2HSV);
		//split(hsvImage, hsvChannel);

		inRange(hsvImage, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), binImage);
		inRange(hsvImage, greenLowScalar, greenHighScalar, greenBinImage);

		morphologyEx(greenBinImage, greenBinImage, MORPH_OPEN, element);
		morphologyEx(greenBinImage, greenBinImage, MORPH_CLOSE, element);

		vector<Point> ball;
		ball.reserve(200);
		scan_row(greenBinImage, ball);
		scan_col(greenBinImage, ball);

		int k = ball.size(), sum_x = 0, sum_y = 0;
		for (int i = 0; i < k; i++) 
		{
			sum_x += ball[i].x;
			sum_y += ball[i].y;
		}
		if (k != 0) {
			sum_x /= k;
			sum_y /= k;
			circle(frame, Point(sum_x, sum_y), 5, Scalar(0, 0, 255), -1);
		}

		imshow("frame", frame);
		imshow("binImage", binImage);
		imshow("greenBinImage", greenBinImage);
		
		char key = 0;
		if (key = waitKey(1) == 27)break;
	}
}

void scan_row(Mat&inputImage, vector<Point>&out) 
{
	for (int i = 0, rows = inputImage.rows, cols = inputImage.cols; i < rows; i++)
	{
		uchar* ptr = inputImage.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			if (*(ptr + j) > 0)
			{
				out.push_back(Point(j, i));
				break;
			}
		}
	}
}

void scan_col(Mat&inputImage, vector<Point>&out)
{
	vector<unsigned char*> ptr;
	ptr.reserve(640);
	for (int i = 0, row = inputImage.rows; i < row; i++)
	{
		ptr.push_back(inputImage.ptr<unsigned char>(i));
	}
	for (int i = 0, row = inputImage.rows, col = inputImage.cols; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			if ((*(ptr[j] + i)) == 255)
			{
				out.push_back(Point(i, j));
				break;
			}
		}
	}
}

