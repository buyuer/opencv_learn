#include "opencv2/opencv.hpp"
#include "vector"
using namespace cv;
using namespace std;

int main(int argc, char **argv) 
{
	/*Mat abc(imread("1.jpg")),out;
	Canny(abc, abc, 3, 9);
	imshow("123", abc);*/
	
	Mat img(imread("3.jpg")), midImage,dstImage;
	Canny(img, midImage, 100, 200);
	cvtColor(midImage, dstImage, CV_GRAY2BGR);
	vector<Vec2f> lines;
	HoughLines(midImage, lines, 1, CV_PI / 180, 150, 0, 0);

	for (size_t i = 0; i < lines.size(); i++) 
	{
		double rho = lines[i][0], theta = lines[i][1]; 
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b)); 
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(dstImage, pt1, pt2, Scalar(0, 255, 255), 1, LINE_AA);
	}
	imshow("123", dstImage);
	waitKey();
	

}