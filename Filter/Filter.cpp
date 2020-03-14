#include "opencv.hpp"
#define Picture "4.jpg"
#define MaxValue 30
using namespace cv;

Mat img, boxFilterOut, blurOut, gaussianBlurOut, medianBlurOut, bilateralFilterOut;
int value;

void callBack(int,void*)
{
	if (value == 0)value = 1;
	Size &cite = Size(value, value);
	
	boxFilter(img, boxFilterOut, -1, cite);
	blur(img, blurOut, cite);
	bilateralFilter(img, bilateralFilterOut, value, value * 2, value / 2);
	if (value % 2 == 0) 
	{
		GaussianBlur(img, gaussianBlurOut, Size(value+1, value+1), 0, 0);
		medianBlur(img, medianBlurOut, value+1);
	}
	else 
	{
		GaussianBlur(img, gaussianBlurOut, cite, 0, 0);
		if (value == 1)value = 3;
		medianBlur(img, medianBlurOut,value);
	}


	imshow("boxFilter--方框滤波", boxFilterOut);
	imshow("blur--均值滤波", blurOut);
	imshow("gassianBlur--高斯滤波", gaussianBlurOut);
	imshow("medianBlur--中值滤波", medianBlurOut);
	imshow("bilateralFilter--双边滤波", bilateralFilterOut);
}
int main() 
{
	img = imread(Picture);
	medianBlurOut=Mat::zeros(img.size(), img.type());
	value = 10;
	namedWindow("原始图", 0);

	createTrackbar("Size值：", "原始图", &value, MaxValue, callBack);

	callBack(1, NULL);
	imshow("原始图", img);
	waitKey();
}