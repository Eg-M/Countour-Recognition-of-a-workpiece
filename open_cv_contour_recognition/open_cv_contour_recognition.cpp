
#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#define PI 3.14159265

using namespace std;
using namespace cv;
int gx=0,gy=0;
bool mouse=0;
int x_pcs=0,y_pcs=0;
// ������� ��������� ��������� ������������ ����
void mouseEvent(int evt, int x, int y, int flags, void* param) 
{                    
    Mat* rgb = (Mat*) param;
    if (evt == CV_EVENT_LBUTTONDOWN) 
    {	
		mouse=1;
		gx=x;
		gy=y;
       
    }  

}

int main() {
	setlocale( LC_ALL,"Russian" );

	try {

		VideoCapture cap(0); // ������� ����������� ������

		// ���� ������ �� ����������
		if (!cap.isOpened())
		{
		cout << "������ �� �������!" << endl;
		Sleep(1500);
			return -1;
		}
		Mat image,grayscale;
		namedWindow("capture", 1);
		  int iSliderValue1 = 80;
		//�������� �������� ��� ��������� ������������� ���������� ����� ��������� �������� �������������
     createTrackbar("���.�����", "capture", &iSliderValue1, 400);

      //�������� �������� ��� ��������� ������������� ���������� ����� ��������� �������� �������������
     int iSliderValue2 = 1;
     createTrackbar("����.�-�", "capture", &iSliderValue2, 20);
	 int dif[4]={0,0,0,0};
	 int flag1=0;
	 double angle=0.0;
		for (;;) {
			int minLineLength=iSliderValue1;
			int maxlengthgap=iSliderValue2;
			
			cap >> image; // ��������� ������ ����������� � ������
			cvtColor(image, grayscale, CV_BGR2GRAY);
			GaussianBlur( grayscale, grayscale,Size(3,3), 2, 2 );
			Canny(grayscale,grayscale, 50, 200, 3 );
			vector<Vec4i> lines;
			
		// ������ �������������� ���� � ��������� ��������
			HoughLinesP(grayscale, lines, 1, CV_PI/180, 80, minLineLength, maxlengthgap );
			for( size_t i = 0; i < lines.size(); i++ )
			  {
			    line(image, Point(lines[i][0], lines[i][1]),Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
				if (abs(lines[i][0]-gx)<3&&abs(lines[i][1]-gy)<3)
				{
					dif[0]=lines[i][0];
					dif[1]=lines[i][1];
					dif[2]=lines[i][2];
					dif[3]=lines[i][3];
					 angle=atan2(abs(dif[3]-dif[1]), abs(dif[2]-dif[0]))* 180 / PI;
					flag1=1;
				}
				if (abs(lines[i][2]-gx)<3&&abs(lines[i][3]-gy)<3)
				{   
					
					dif[0]=lines[i][0];
					dif[1]=lines[i][1];
					dif[2]=lines[i][2];
					dif[3]=lines[i][3];
					
					 angle=atan2(abs(dif[3]-dif[1]), abs(dif[2]-dif[0]))* 180 / PI;
					flag1=2;

				}
				// ����� ���������� � ���������� �������� ����� � ���� �������� ��������� � ��������� ������
				if(mouse)
				{
				if (flag1==1)
				{
				printf("%d %d   %f\n",  dif[0], dif[1],angle); 
				x_pcs=dif[0];
				y_pcs=dif[1];
				}
			    if(flag1==2)
				{
				printf("%d %d   %f\n",  dif[2],dif[3],angle); 
				x_pcs=dif[2];
				y_pcs=dif[3];
				}

				flag1=0;
				mouse=0;
				}
				
			  }
		
			
			imshow("capture",image);
			setMouseCallback("capture", mouseEvent, &image);
			 // ���������� ��������� ����� ��������� ����� ������� �������  Enter ����� 30 �����������
			if (waitKey(30) >= 0)
				break;
		}
		vector<int> attribute(2);
		attribute[0] = CV_IMWRITE_JPEG_QUALITY;
		attribute[1] = 90;
		// ���������� ����������� � ���-������ � ������������� ���������
		imwrite(string("capture.jpg"), image, attribute);
		ofstream outputfile;
		//������ ���������� � ���������� ����� ���� ��������� � ���� �������� ��������� � �������� � ���� data.txt
		 outputfile.open("data.txt");
         outputfile << x_pcs;
		 outputfile << " ";
		 outputfile << y_pcs;
		 outputfile << " ";
		 outputfile << angle<<endl;
		return 0;
	} catch (string msg) {
		cerr << "���������� ��������� �����������" << endl << msg << endl;
		return -1;
	}
}