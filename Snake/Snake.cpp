/*??????????????????????????????????????¦Ë???????????*/

#include "opencv2/opencv.hpp"
#include "vector"
#include "windows.h"
#define background   "background.jpg"		
using namespace cv;
using namespace std;

std::vector<cv::Point> g_Location(3);

Mat img1;
Point /*g_Location[100],*/ foodLocation;
uchar g_R = 5;
uchar g_SnakeLength = 3;
uchar g_SnakeDirection = 2;
uchar g_GameSpeed = 100;
int g_MapLength, g_MapWidth;

int randomNUM() 
{
	int num;
    num = (int) (((rand() % 100) / 100.0) * 80);
	return num;
}
void DrawCircle(Mat img, Point center,Scalar scalar)
{
	circle(img,center,g_R,scalar,-1,8);
}
void drawSnake(unsigned char num)
{
	img1 = imread(background);
	DrawCircle(img1, foodLocation, Scalar(0, 0, 255));
	DrawCircle(img1, g_Location[0], Scalar(0, 255, 12));
	for (int i = 1;i < num; i++)
	{
		DrawCircle(img1, g_Location[i],Scalar(255,0,255));
	}
	namedWindow("?????", WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
	cvResizeWindow("?????", img1.cols, img1.rows);
	imshow("?????", img1);
}
void locationExchange(uchar num)
{
	for (int i = 1; i < num; i++)
	{
		g_Location[num - i] = g_Location[num - i - 1];
	}
}
void showTest(char *str) 
{
	int baseline;
	Size text_size = getTextSize(str, FONT_HERSHEY_PLAIN, 4, 8,&baseline);
	putText(img1, str, Point(img1.cols/2-text_size.width / 2, img1.rows/2 + text_size.height / 2), FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 255),8, 8, 0);
	namedWindow("?????", WINDOW_NORMAL);
	cvResizeWindow("?????", img1.cols, img1.rows);
	imshow("?????", img1);
}
void gameOver()
{
	for (int i = 1; i < g_SnakeLength; i++)
	{
		if (g_Location[0].x == g_Location[i].x && g_Location[0].y == g_Location[i].y)
		{
			while (1)
			{
				showTest("Game Over");
				if (waitKey() == 27) exit(0);
			}
		}
	}
	if (g_Location[0].x < 5 || g_Location[0].x>g_MapLength || g_Location[0].y < 5 || g_Location[0].y>g_MapWidth)
	{
		while (true)
		{
			showTest("Game Over");
			if (waitKey() == 27) exit(0);
		}
	}
}
void SnakeMove()
{
	uchar keyTemp = waitKey(g_GameSpeed);
	switch (keyTemp)
	{
	case 'w':if (g_SnakeDirection !=2)
	         { 
		          locationExchange(g_SnakeLength); 
				  g_SnakeDirection = 1; g_Location[0].y -= 2 * g_R; gameOver(); drawSnake(g_SnakeLength);
	         }  break;
	case 's':if (g_SnakeDirection !=1)
	         {
		          locationExchange(g_SnakeLength); 
				  g_SnakeDirection = 2; g_Location[0].y += 2 * g_R; gameOver(); drawSnake(g_SnakeLength); 
	         }  break;
	case 'a':if (g_SnakeDirection !=4)
			 {
				  locationExchange(g_SnakeLength);
				  g_SnakeDirection = 3; g_Location[0].x -= 2 * g_R; gameOver(); drawSnake(g_SnakeLength); 
			 }  break;
	case 'd':if (g_SnakeDirection !=3)
	         {
		          locationExchange(g_SnakeLength);
		          g_SnakeDirection = 4; g_Location[0].x += 2 * g_R; gameOver(); drawSnake(g_SnakeLength); 
	          }  break;
	case 27 :exit(0); break;
	case 'p':while (true) { showTest("Pausing"); if (waitKey() == 'p') { break; } }break;
	case 'q': g_GameSpeed += 200; if (g_GameSpeed > 900){g_GameSpeed = 100;}break;
	default:
		locationExchange(g_SnakeLength);
		if (g_SnakeDirection == 1) { g_Location[0].y -= 2 * g_R;}
		if (g_SnakeDirection == 2) { g_Location[0].y += 2 * g_R;}
		if (g_SnakeDirection == 3) { g_Location[0].x -= 2 * g_R;}
		if (g_SnakeDirection == 4) { g_Location[0].x += 2 * g_R;}
		gameOver();
		drawSnake(g_SnakeLength);
		break;
	}
}
void SnakeInit()
{
	printf("*************************************??????***********************************************************\n\n");
	printf("????????????????????????????? backGround.jpg ??????????????????????????\n\n");
	printf("'w'??'a'??'s'??'d' ?????????????????????????\n\n");
	printf("????§µ?????p????????   Esc????????    q?????????\n\n");
	printf("********************************************************************************************************\n\n");
	Sleep(2000);
	img1 = imread(background);
	showTest("Press any key to start");
	g_MapLength = img1.cols - g_R;
	g_MapWidth = img1.rows - g_R;
	g_Location[0] = Point(25, 5);
	g_Location[1] = Point(15, 5);
	g_Location[2] = Point(5, 5);
	srand((int)time(0));
	foodLocation = Point(randomNUM()*10+5, randomNUM() * 10+5);
	waitKey(0);
}
int main()
{
	SnakeInit();
	while (true)
	{	
		SnakeMove();
		if (g_Location[0].x == foodLocation.x && g_Location[0].y == foodLocation.y)
		{
			g_SnakeLength++;//Point SnakeTail;
			g_Location.push_back(Point(0,0));
			foodLocation = Point(randomNUM() * 10 + 5, randomNUM() * 10 + 5);
		}	
	}
}