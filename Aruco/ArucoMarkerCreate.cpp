#include "opencv2/opencv.hpp"
#include "opencv2/aruco.hpp"
#include "iostream"


using namespace cv;
using namespace std;
using namespace aruco;

typedef struct 
{
	int imageSize = 500;
	int dictionarySize = 50;
	int markerSize = 4;
}inOptions,*inOptionsP;

void draw_marker(Ptr<cv::aruco::Dictionary> &dictionary,String str, int imageSize, int markerNum);


int main(int argc,char** argv) 
{
	int imageSize = 500;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-help"))
		{
			cout << "参数说明：" << endl;
			cout << "       -help    帮助" << endl;
			cout << "       -marker  生成marker图像，可选四种:" << endl;
			cout << "                             DICT_4X4_50 " << endl;
			cout << "                             DICT_5X5_50 " << endl;
			cout << "                             DICT_6X6_50 " << endl;
			cout << "                             DICT_7X7_50 " << endl;
		}
		if (!strcmp(argv[i], "-marker")) 
		{
			if (i + 1 <= argc) 
			{
				if (!strcmp(argv[i + 1], "DICT_4X4_50"))
				{
					Ptr<cv::aruco::Dictionary> dictionary(getPredefinedDictionary(DICT_4X4_50));
					draw_marker(dictionary,String("DICT_4X4_50_"), imageSize, 50);
				}
				else if (!strcmp(argv[i + 1], "DICT_5X5_50"))
				{
					Ptr<cv::aruco::Dictionary> dictionary(getPredefinedDictionary(DICT_5X5_50));
					draw_marker(dictionary, String("DICT_5X5_50_"), imageSize, 50);
				}
				else if (!strcmp(argv[i + 1], "DICT_6X6_50"))
				{
					Ptr<cv::aruco::Dictionary> dictionary(getPredefinedDictionary(DICT_6X6_50));
					draw_marker(dictionary, String("DICT_6X6_50_"), imageSize, 50);
				}
				else if (!strcmp(argv[i + 1], "DICT_7X7_50"))
				{
					Ptr<cv::aruco::Dictionary> dictionary(getPredefinedDictionary(DICT_7X7_50));
					draw_marker(dictionary, String("DICT_7X7_50_"), imageSize, 50);
				}
				else
				{
					cout << "参数不对" << endl;
				}
			}
		
		}

	}

}

void draw_marker(Ptr<cv::aruco::Dictionary> &dictionary, String str, int imageSize, int markerNum)
{
	Mat markerImage;
	for (int i = 0; i < markerNum; i++)
	{
		drawMarker(dictionary, i, imageSize, markerImage, 1);
		//imshow("makerImage", markerImage);
		String tempStr = str + to_string(i) + String(".jpg");
		//waitKey();
		imwrite(tempStr, markerImage);
	}
}


