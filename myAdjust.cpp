#include<iostream>
#include<opencv2\highgui.hpp>
#include<cv.h>
#include<opencv2\imgproc.hpp>//threshhold

using namespace std;
using namespace cv;
int value = 90;//二值化阈值

vector<Mat> bgr;//存储图像分割出来的r\g\b通道图像
Mat local;
void thresh_callback(int, void*);

int myadjust(void)
{
	Mat img,con,rlocal;
	vector<vector<Point>> contours;

	img = imread("奥巴马.jpg");

	split(img, bgr);
	//imshow("b", bgr[0]);
	
	threshold(bgr[0], local, 90, 255, CV_THRESH_BINARY);//二值化图片，并且反色
	rlocal = 255 - local;//由于findContours是作用于白色，而这里字体部分是黑色

	findContours(rlocal, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cout << "contours size:" << contours.size() << endl;
	int j = 0;
	vector<Rect> rectall;
	int l = img.cols , r=0, d=0, u=img.rows;//分别用于标记提取出来的所有框的最左、右、上、下像素

	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect;
		rect=boundingRect(contours[i]);//矩形边界拟合，为每一个框找出一个最临界的正着放的矩形框
		rectall.push_back(rect);
		if (rect.height*rect.width > 30 && rect.height*rect.width < 800)//矩形面积，经验值
		{
			if (rect.x < l)
				l = rect.x;
			if (rect.x+rect.width-1 > r)
				r = rect.x + rect.width - 1;
			if (rect.y < u)
				u = rect.y;
			if (rect.y+rect.height-1 > d)
				d = rect.y + rect.height - 1;
			rectangle(img, rect, Scalar(255, 0, 0));//用蓝色框出每一个处理出来的字的位置
			j++;		
		}
		
	}
	if (l > 1) l -= 2;
	if (u > 1)u -= 2;
	if (r < img.cols - 2) r += 2;
	if (d < img.rows - 2) d += 2;//稍微扩大下红色框的位置，不要那么贴合
	rectangle(img, Rect(l, u, r - l + 1, d - u + 1), Scalar(0, 0, 255));//红色框出数字总信息区域,稍微

	cout <<j<<endl;
	namedWindow("原图");
	imshow("原图", img);
	namedWindow("二值化");
	imshow("二值化", local);
	createTrackbar("滑条", "二值化", &value, 255, thresh_callback);//这里显示出来方便确定阈值
	
	waitKey();
	imwrite("奥巴马识别效果.jpg", img);
	return 0;
}
void thresh_callback(int, void*)
{
	threshold(bgr[0], local, value, 255, CV_THRESH_BINARY);
	imshow("二值化", local);
}

int main(void)
{
	myadjust();
	return 0;
}
