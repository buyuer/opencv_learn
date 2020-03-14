#include "opencv.hpp"
#define valueMAX 30               //滑动条最大值
#define Picture "5.jpg"      //要加载的图片
using namespace cv;
Mat img, dilateOut, erodeOut, openOut, closeOut, gradientOut, topOut, blackOut,element,&cite=element;
int value;
void callBack(int, void *) 
{
	if (value == 0)value = 1;
	element = getStructuringElement(MORPH_RECT, Size(value, value));
	dilate(img, dilateOut, cite);
	erode(img, erodeOut, cite);
	morphologyEx(img, openOut, MORPH_OPEN, cite);
	morphologyEx(img, closeOut, MORPH_CLOSE, cite);
	morphologyEx(img, gradientOut, MORPH_GRADIENT, cite);
	morphologyEx(img, topOut, MORPH_TOPHAT, cite);
	morphologyEx(img, blackOut, MORPH_BLACKHAT, cite);

	imshow("dilate--膨胀", dilateOut);
	imshow("erode--腐蚀", erodeOut);
	imshow("opening operation--开运算", openOut);
	imshow("closing operation--闭运算", closeOut);
	imshow("morpholoty gradient--形态学梯度", gradientOut);
	imshow("top hat--顶帽", topOut);
	imshow("black hat--黑帽", blackOut);
}
int main() 
{
	img = imread(Picture);
	value = 10;
	namedWindow("原图", 0);
	createTrackbar("内核值", "原图", &value, valueMAX, callBack);
	callBack(1, NULL);
	imshow("原图", img);
	waitKey();
}