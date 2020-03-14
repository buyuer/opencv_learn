#include "opencv.hpp"
#include "windows.h"

using namespace cv;
Mat original, mask, img,midimg;
Point p1, p2;
char lineSize = 8;
std::string str;
void mouseCallBack(int event, int x, int y, int flags, void* param)
{
	if (event == EVENT_MOUSEMOVE && (flags&EVENT_FLAG_LBUTTON))
	{
		line(mask, Point(x, y), Point(x,y), Scalar::all(255), lineSize, 8);
		line(img, Point(x, y), Point(x,y), Scalar::all(255), lineSize, 8);
		imshow("图像修复", img);
	}
}
int main()
{
	std::cout << "---------------【使用说明】---------------\n1、按住鼠标左键对要修复的地方进行选中，被选中的地方将变成白色。\n2、按下空格，将对照片进行修复，按下小写s将会对当前图片进行保存，文件存储在程序目录下，文件名为Inpainting.jpg\n3、如不小心选错，可按小写r可恢复原状态\n4、按小写x可以改变涂抹线的粗细\n------------------------------------------\n";
	std::cout << "请把要处理的图片放到程序的目录下，然后输入要处理的文件名。\n\n也可以直接输入图片路径。\n\n请输入路径或文件名：       (注意文件名要由扩展名如：3.jpg或f:\\picture\\3.jpg)\n";
	system("color 1E");
	std::cin >> str;
	original = imread(str);
	while (!original.data) 
	{ 
		std::cout << "请确定输入的文件名或路径是否正确，并重新输入：\n";
		str = "";
		std::cin >> str;
		original = imread(str);
	}
	original.copyTo(img);
	original.copyTo(midimg);
	mask = Mat(original.size(), original.type());		
	cvtColor(mask, mask, CV_BGR2GRAY);
	namedWindow("图像修复", 0);
	imshow("图像修复", img);
	setMouseCallback("图像修复", mouseCallBack);
	while (1)
	{
		switch (waitKey(0))
		{
		case ' ':
			inpaint(midimg, mask, midimg, 3, INPAINT_TELEA);
			mask = Scalar::all(0);
			imshow("图像修复", midimg);
			break;
		case 'r':
			original.copyTo(img);
			mask = Scalar::all(0);
			imshow("图像修复", img);
			break;
		case 's':
			imwrite("Inpainting.jpg", midimg);
			MessageBox(NULL, TEXT("图片已保存，请查看"), TEXT("提示"), MB_OK);
			exit(0);
		case 'x':
			lineSize++;
			if (lineSize > 8)lineSize = 1;
			break;
		case 27:
			exit(0);
		default:
			break;
		}
	}
}