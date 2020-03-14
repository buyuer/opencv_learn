/*
*	Author:XintongDing
*	Date  :2018-12-18
*/

#include "opencv2/opencv.hpp"
#include "iostream"

using namespace cv;
using namespace std;

int main()
{
	VideoCapture camera;
	Mat frame, grayFrame, thresholdFrame, resFrame;

	const char str[8][2] = { "u","p","r","e","U","P", "R", "E" };
	RNG rng(123);

	char key = 0;
	int thresholdValue = 100;
	int mode = 0;

	camera.open(0);

	namedWindow("charFrame");

	createTrackbar("threshold", "charFrame", &thresholdValue, 255, [](int, void*)->void {});
	createTrackbar("mode", "charFrame", &mode, 1, [](int, void*)->void {});

	while (camera.grab())
	{
		camera.operator>> (frame);
		Mat charFrame(frame.size(), CV_8UC1, Scalar::all(0));
		cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
		threshold(grayFrame, thresholdFrame, thresholdValue, 255, THRESH_BINARY);
		//Canny(thresholdFrame, resFrame, 30, 90);
		resFrame = thresholdFrame;

		for (int row = 0; row < resFrame.rows; row += 5)
		{
			unsigned char* rowP = resFrame.ptr<unsigned char>(row);
			for (int col = 0; col < resFrame.cols; col += 10)
			{
				if (*(rowP + col) == 255 * mode)
				{
					putText(charFrame, str[rng.uniform(0, sizeof(str) / 2)], Point(col, row), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255));
				}
			}
		}

		//imshow("Canny", resFrame);
		imshow("frame", frame);
		imshow("charFrame", charFrame);

		key = waitKey(1);
		if (key == 27)break;
	}

	return 0;
}

