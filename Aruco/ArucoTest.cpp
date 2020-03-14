#include "opencv2/opencv.hpp"
#include "opencv2/aruco.hpp"
#include "iostream"
#include "cmath"

using namespace cv;
using namespace std;
using namespace aruco;


Mat g_CameraMatrix(3, 3, CV_32FC1, Scalar::all(0));
Mat g_DistCoeffs(1, 5, CV_32FC1, Scalar::all(0));

void Camera_Argument_Init();

int main(int argc, char** argv) 
{
	Mat markerImage;
	Ptr<Dictionary> dic = getPredefinedDictionary(DICT_6X6_50);
	drawMarker(dic, 23, 600, markerImage,1);
	GaussianBlur(markerImage, markerImage, Size(3, 3), 0.0, 0.0);
	imshow("marker", markerImage);

	Mat camera;
	VideoCapture inputVideo;
	
	Camera_Argument_Init();
	inputVideo.open(0);
	while (inputVideo.grab()) 
	{
		vector<int> ids;
		vector<vector<Point2f>> markerCorners;
		inputVideo >> camera;
		GaussianBlur(camera, camera, Size(7, 7), 0.0, 0.0);
		detectMarkers(camera, dic, markerCorners, ids);
		if (ids.size() > 0) 
		{
			vector<Vec3d> rvecs, tvecs;
			
			estimatePoseSingleMarkers(markerCorners, 200, g_CameraMatrix, g_DistCoeffs, rvecs, tvecs);

			drawDetectedMarkers(camera, markerCorners, ids);

			drawAxis(camera, g_CameraMatrix, g_DistCoeffs, rvecs[0], tvecs[0], 100);

			cout << rvecs[0][0] << "     \t X:" << tvecs[0][0] << endl;
			cout << rvecs[0][1] << "     \t Y:" << tvecs[0][1] << endl;
			cout << rvecs[0][2] << "     \t Z:" << tvecs[0][2] << endl << endl;
		}	
		imshow("camera", camera);
		if (waitKey(1) == 27)break;
	}
}

void Camera_Argument_Init() 
{
	g_CameraMatrix.at<float>(0, 0) = 464.2836610476581F;    //十字线
	g_CameraMatrix.at<float>(0, 1) = 0.0F;
	g_CameraMatrix.at<float>(0, 2) = 293.5407750754699F;
	g_CameraMatrix.at<float>(1, 0) = 0.0F;
	g_CameraMatrix.at<float>(1, 1) = 465.6682526001514F;
	g_CameraMatrix.at<float>(1, 2) = 230.0712285677754F;
	g_CameraMatrix.at<float>(2, 0) = 0.0F;
	g_CameraMatrix.at<float>(2, 1) = 0.0F;
	g_CameraMatrix.at<float>(2, 2) = 1.0F;
	g_DistCoeffs.at<float>(0, 0) = 0.007364832957867318F;
	g_DistCoeffs.at<float>(0, 1) = -0.1181486079046361F;
	g_DistCoeffs.at<float>(0, 2) = 0.0007043064693464629F;
	g_DistCoeffs.at<float>(0, 3) = -0.002628729642784704F;
	g_DistCoeffs.at<float>(0, 4) = 0.4768385629061455F;
	
	g_CameraMatrix.at<float>(0, 0) = 596.9163317751921F;    //电脑相机
	g_CameraMatrix.at<float>(0, 1) = 0.0F;
	g_CameraMatrix.at<float>(0, 2) = 309.0472112191528F;
	g_CameraMatrix.at<float>(1, 0) = 0.0F;
	g_CameraMatrix.at<float>(1, 1) = 597.5742611550378F;
	g_CameraMatrix.at<float>(1, 2) = 243.9901882097711F;
	g_CameraMatrix.at<float>(2, 0) = 0.0F;
	g_CameraMatrix.at<float>(2, 1) = 0.0F;
	g_CameraMatrix.at<float>(2, 2) = 1.0F;
	g_DistCoeffs.at<float>(0, 0) = 0.0754296440218083F;
	g_DistCoeffs.at<float>(0, 1) = 0.1494890720447138F;
	g_DistCoeffs.at<float>(0, 2) = -0.00476949068252268F;
	g_DistCoeffs.at<float>(0, 3) = -0.001084435678481666F;
	g_DistCoeffs.at<float>(0, 4) = -2.338167693811399F;
}
