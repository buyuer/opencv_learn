#include "opencv2/opencv.hpp"
#include "vector"
using namespace cv;
using namespace std;

Size g_CornersSize = Size(11, 8);
Size g_ChessSize = Size(10, 10);
Size g_ImageSize;
Mat g_CameraMatrix(3, 3, CV_32FC1, Scalar::all(0)); 
Mat g_DistCoeffs(1, 5, CV_32FC1, Scalar::all(0));
vector<Mat> g_tvecsMat, g_rvecsMat;
vector<vector<Point2f>> g_Corners;
int g_imageCount = 0;
void Find_Chess_Board_Corners(Mat& image, vector<vector<Point2f>>& corners);
void Calibrate_Camera(char num);
void Get_Camera_Argument();
int main(int argc, char** argv) 
{
	int cameraNum = 1;
	Calibrate_Camera(cameraNum);
}

void Calibrate_Camera(char num)
{
	cout << "按下空格采集一张照片" << endl << "按下v计算相机内存和畸变并显示校正后镜头" << endl << "按下esc退出" << endl;
	VideoCapture camera;
	Mat image;
	Mat image1;
	char key;
	camera.open(num);
	camera.set(CAP_PROP_FRAME_WIDTH, 640);
	camera.set(CAP_PROP_FRAME_HEIGHT, 480);
	camera >> image;
	g_ImageSize = image.size();
	bool flag = 0;
	cout << "图像是分辨率：" << image.size() << endl;
	while (true)
	{
		camera >> image;
		if (flag == 1) 
		{
			undistort(image, image1, g_CameraMatrix, g_DistCoeffs,g_CameraMatrix);
			imshow("Camera", image1);
		}
		else
		{
			imshow("Camera", image);
		}
		key = waitKey(30);
		if (key == ' ') 
		{
			Find_Chess_Board_Corners(image, g_Corners);
		}
		if (key == 'v') 
		{
			vector<vector<Point3f>> object_points;
			vector<int> point_couts;
			int i, j, t;
			for (t = 0; t < g_imageCount; t++)
			{
				vector<Point3f> temp_Points;
				for (i = 0; i < g_CornersSize.height; i++)
				{
					for (j = 0; j < g_CornersSize.width; j++)
					{
						Point3f realPoint;
						realPoint.x = i*g_ChessSize.width;
						realPoint.y = j*g_ChessSize.height;
						realPoint.z = 0;
						temp_Points.push_back(realPoint);
					}
				}
				object_points.push_back(temp_Points);
			}
			for (i = 0; i < g_imageCount; i++)
			{
				point_couts.push_back(g_ChessSize.width*g_ChessSize.height);
			}
			calibrateCamera(object_points, g_Corners, g_ImageSize, g_CameraMatrix, g_DistCoeffs, g_rvecsMat, g_tvecsMat, 0);
			cout << "已计算得到的相机参数: " << endl;
			cout << "相机内参：" << endl << g_CameraMatrix << endl;
			cout << "相机畸变：" << endl << g_DistCoeffs << endl;
			flag = 1;
		}
		if (key == 27) 
		{
			break;
		}
	}
	//exit(1);
}

void Find_Chess_Board_Corners(Mat& image, vector<vector<Point2f>>& corners) 
{
	vector<Point2f> tempCorners;
	if (findChessboardCorners(image, g_CornersSize, tempCorners) == 0) 
	{
		cout << "没有找到角点" << endl;
	}
	else 
	{
		Mat tempImage;
		cvtColor(image, tempImage, COLOR_BGR2GRAY);
		find4QuadCornerSubpix(tempImage, tempCorners, Size(5, 5));
		g_Corners.push_back(tempCorners);
		drawChessboardCorners(image, g_CornersSize, g_Corners[g_imageCount++], true);
		imshow("效果图", image);
		cout << "已采集" << g_imageCount << "张图片" << endl;
	}
}

void Get_Camera_Argument() 
{
	vector<vector<Point3f>> object_points;
	vector<int> point_couts;
	int i, j, t;
	for (t = 0; t < g_imageCount; t++) 
	{
		vector<Point3f> temp_Points;
		for (i = 0; i < g_CornersSize.height; i++) 
		{
			for (j = 0; j < g_CornersSize.width; j++) 
			{
				Point3f realPoint;
				realPoint.x = i*g_ChessSize.width;
				realPoint.y = j*g_ChessSize.height;
				realPoint.z = 0;
				temp_Points.push_back(realPoint);
			}
		}
		object_points.push_back(temp_Points);
	}
	for (i = 0; i < g_imageCount; i++) 
	{
		point_couts.push_back(g_ChessSize.width*g_ChessSize.height);
	}
	calibrateCamera(object_points, g_Corners, g_ImageSize, g_CameraMatrix, g_DistCoeffs, g_rvecsMat, g_tvecsMat, 0);
	cout << "已计算得到的相机参数: " << endl;
	cout << "相机内参：" << endl << g_CameraMatrix << endl;
	cout << "相机畸变：" << endl << g_DistCoeffs << endl;
}