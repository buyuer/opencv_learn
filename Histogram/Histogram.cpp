#include "opencv.hpp"
#include "Vector"
using namespace cv;
Mat g_srcImage, g_hsvImage, g_hueImage;
int g_bins = 30;
void drawHistogram();
void showRGBHist();
void H_S_HIST();

void on_BinChange(int, void*);
void opposite();
void matching();
int main() 
{
	//drawHistogram();
	//H_S_HIST();
	//opposite();
	matching();
	waitKey();
}



void H_S_HIST()
{
	Mat srcImage, hsvImage;
	srcImage = imread("1.jpg");
	cvtColor(srcImage, hsvImage, COLOR_BGR2HSV);

	int hueBinNum = 30;
	int saturationBinNum = 32;
	int histSize[] = { hueBinNum,saturationBinNum };
	float hueRange[] = { 0,180 };
	float saturationRanges[] = { 0,256 };
	const float *ranges[] = { hueRange,saturationRanges };
	MatND dstHist;

	int channels[] = { 0,1 };
	calcHist(&hsvImage, 1, channels, Mat(), dstHist, 2, histSize, ranges, true, false);
	double maxValue = 0;
	minMaxLoc(dstHist, 0, &maxValue, 0, 0);
	int scale = 10;

	Mat hisImg = Mat::zeros(saturationBinNum*scale, hueBinNum*scale, CV_8UC3);

	for (int hue = 0; hue < hueBinNum; hue++)
	{
		for (int saturation = 0; saturation < saturationBinNum; saturation++)
		{
			float binValue = dstHist.at<float>(hue, saturation);
			int intensity = cvRound(binValue * 255 / maxValue);

			rectangle(hisImg, Point(hue*scale, saturation*scale), Point(((hue + 1)*scale - 1), (saturation + 1)*scale - 1), Scalar::all(intensity), FILLED);
		}

	}
	imshow("素材图", srcImage);
	imshow("H-S直方图", hisImg);
	//waitKey();

}
void showRGBHist()
{
	Mat img(imread("1.jpg"));
	std::vector<Mat> channels(3);
	split(img, channels);
	for (int i = 0; i < 3; i++)
	{
		imshow("" + i, channels[0]);
	}
}
void opposite()
{
	g_srcImage = imread("3.jpg", 1);
	cvtColor(g_srcImage, g_hsvImage, COLOR_BGR2HSV);

	g_hueImage.create(g_hsvImage.size(), g_hsvImage.depth());
	int ch[] = { 0,0 };
	//mixChannels(&g_hsvImage, 1, &g_hueImage, 1, ch, 1);

	namedWindow("原始图");
	createTrackbar("色调组距", "原始图", &g_bins, 180, on_BinChange);
	on_BinChange(0, NULL);
	imshow("原始图", g_srcImage);
}
void on_BinChange(int,void*) 
{
	MatND hist;
	int histSize = MAX(g_bins, 2);
	float hue_range[] = { 0,180 };
	const float* ranges = { hue_range };
	calcHist(&g_hueImage, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	MatND backproj;
	calcBackProject(&g_hueImage, 1, 0, hist, backproj, &ranges, 1, true);

	imshow("反向投影图", backproj);
}
void drawHistogram()
{
	/*drawHistogram(imread("1.jpg", 0));
	waitKey();*/
	Mat srcImage = imread("2.jpg", 0);
	imshow("原图", srcImage);
	MatND dstHist;
	int dims = 1;
	float hranges[] = { 0,255 };
	const float *ranges[] = { hranges };
	int size = 256;
	int channels = 0;

	calcHist(&srcImage, 1, &channels, Mat(), dstHist, dims, &size, ranges);
	int scale = 1;
	Mat dstImage(size*scale, size, CV_8U, Scalar(0));
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);
	int hpt = saturate_cast<int>(1.0*size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);
		int realValue = saturate_cast<int>(binValue*hpt / maxValue);
		rectangle(dstImage, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue), Scalar(i));
	}
	imshow("一维直方图", dstImage);
	//imshow("123", dstHist);
	//waitKey();
}
void matching() 
{
	VideoCapture video;
	video.open(0);
	while (1) {
		Mat  templateImage, resultImage;
		Mat srcImage;
		//srcImage = imread("1.jpg");
		video >> srcImage;
		templateImage = imread("4.jpg");
		imshow("template", templateImage);
		namedWindow("原始图");
		namedWindow("效果图");
		resultImage.create(srcImage.rows - templateImage.rows + 1, srcImage.cols - templateImage.cols + 1, CV_32FC1);
		matchTemplate(srcImage, templateImage, resultImage, 0);
		normalize(resultImage, resultImage, 0, 1, NORM_MINMAX, -1, Mat());
		                                                                             
		double minValue, maxValue;
		Point minLocation, maxLocation, matchLocation;
		minMaxLoc(resultImage, &minValue, &maxValue, &minLocation, &maxLocation, Mat());
		matchLocation = minLocation;
		std::cout << matchLocation << std::endl;
		rectangle(resultImage, matchLocation, Point(matchLocation.x + templateImage.cols, matchLocation.y + templateImage.rows), Scalar(0, 0, 255), 2, 8, 0);
		std::cout << Point(matchLocation.y + templateImage.rows, matchLocation.x + templateImage.cols);
		rectangle(srcImage, matchLocation, Point(matchLocation.x + templateImage.cols, matchLocation.y + templateImage.rows), Scalar(0, 0, 255), 2, 8, 0);

		imshow("效果图", resultImage);
		imshow("原始图", srcImage);
		waitKey(100);
	}
}