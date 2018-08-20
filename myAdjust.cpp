#include<iostream>
#include<opencv2\highgui.hpp>
#include<cv.h>
#include<opencv2\imgproc.hpp>//threshhold

using namespace std;
using namespace cv;
int value = 90;//��ֵ����ֵ

vector<Mat> bgr;//�洢ͼ��ָ������r\g\bͨ��ͼ��
Mat local;
void thresh_callback(int, void*);

int myadjust(void)
{
	Mat img,con,rlocal;
	vector<vector<Point>> contours;

	img = imread("�°���.jpg");

	split(img, bgr);
	//imshow("b", bgr[0]);
	
	threshold(bgr[0], local, 90, 255, CV_THRESH_BINARY);//��ֵ��ͼƬ�����ҷ�ɫ
	rlocal = 255 - local;//����findContours�������ڰ�ɫ�����������岿���Ǻ�ɫ

	findContours(rlocal, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cout << "contours size:" << contours.size() << endl;
	int j = 0;
	vector<Rect> rectall;
	int l = img.cols , r=0, d=0, u=img.rows;//�ֱ����ڱ����ȡ���������п�������ҡ��ϡ�������

	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect;
		rect=boundingRect(contours[i]);//���α߽���ϣ�Ϊÿһ�����ҳ�һ�����ٽ�����ŷŵľ��ο�
		rectall.push_back(rect);
		if (rect.height*rect.width > 30 && rect.height*rect.width < 800)//�������������ֵ
		{
			if (rect.x < l)
				l = rect.x;
			if (rect.x+rect.width-1 > r)
				r = rect.x + rect.width - 1;
			if (rect.y < u)
				u = rect.y;
			if (rect.y+rect.height-1 > d)
				d = rect.y + rect.height - 1;
			rectangle(img, rect, Scalar(255, 0, 0));//����ɫ���ÿһ������������ֵ�λ��
			j++;		
		}
		
	}
	if (l > 1) l -= 2;
	if (u > 1)u -= 2;
	if (r < img.cols - 2) r += 2;
	if (d < img.rows - 2) d += 2;//��΢�����º�ɫ���λ�ã���Ҫ��ô����
	rectangle(img, Rect(l, u, r - l + 1, d - u + 1), Scalar(0, 0, 255));//��ɫ�����������Ϣ����,��΢

	cout <<j<<endl;
	namedWindow("ԭͼ");
	imshow("ԭͼ", img);
	namedWindow("��ֵ��");
	imshow("��ֵ��", local);
	createTrackbar("����", "��ֵ��", &value, 255, thresh_callback);//������ʾ��������ȷ����ֵ
	
	waitKey();
	imwrite("�°���ʶ��Ч��.jpg", img);
	return 0;
}
void thresh_callback(int, void*)
{
	threshold(bgr[0], local, value, 255, CV_THRESH_BINARY);
	imshow("��ֵ��", local);
}

int main(void)
{
	myadjust();
	return 0;
}
