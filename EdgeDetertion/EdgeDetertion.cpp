#include "opencv.hpp"

using namespace cv;

void CannyShow(Mat &img,int threshold)
{
	Mat edge, out;
	cvtColor(img, out, COLOR_BGR2GRAY);
	blur(out, edge, Size(3, 3));
	Canny(edge, edge, threshold, threshold*3, 3);
	img.copyTo(out, edge);
	imshow("效果图", out);
	imshow("效果图edge", edge);
	imshow("原始图", img);
}
void SobleShow(Mat &img,int Ksize) 
{
	Mat X, Y, XY;
	imshow("原始图", img);

	Sobel(img, X, CV_16S, 1, 0, Ksize);
	convertScaleAbs(X, X);
	imshow("Soble absX", X);

	Sobel(img, Y, CV_16S, 0, 1, Ksize);
	convertScaleAbs(Y, Y);
	imshow("Soble absY", Y);

	addWeighted(X, 0.5, Y, 0.5, 0, XY);
	imshow("Sobel XY", XY);
}
void LaplacianShow(Mat &img) 
{
	Mat out,abs_out;
	blur(img, out, Size(3, 3));
	cvtColor(out, out, COLOR_BGR2GRAY);
	Laplacian(out, abs_out, CV_16S, 3, 1, 0, 4);  //不支持就地
	convertScaleAbs(abs_out, abs_out);
	imshow("原始图", img);
	imshow("效果图", abs_out);
}
void ScharrShow(Mat &img) 
{
	Mat X, Y, XY;

	imshow("原始图", img);
	
	Scharr(img, X, CV_16S, 1, 0);
	convertScaleAbs(X, X);
	imshow("Scharr absX", X);

	Scharr(img, Y, CV_16S, 0, 1);
	convertScaleAbs(Y, Y);
	imshow("Scharr absY", Y);

	addWeighted(X, 0.5, Y, 0.5, 0, XY);
	imshow("Scharr absXY", XY);

}
int main(int argc, char **argv) 
{
	CannyShow(imread("1.jpg"),40);
	//SobleShow(imread("2.jpg"),3);
	//LaplacianShow(imread("3.jpg"));
	//ScharrShow(imread("1.jpg"));
	waitKey();
}