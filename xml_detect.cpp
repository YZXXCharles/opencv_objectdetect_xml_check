#include <windows.h>  
#include <mmsystem.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include "wininet.h"  
#include <direct.h>  
#include <string.h>  
#include <list>  
#pragma comment(lib,"Wininet.lib")  

#include "opencv2/opencv.hpp" 
#include "opencv2/objdetect/objdetect.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/ml/ml.hpp"  
#include <opencv2/imgproc/types_c.h>
#include <iostream>  
#include <stdio.h>  

using namespace std;
using namespace cv;

String cascadeName = "C:/souhudet/cascade.xml";//ѵ������  

int main()
{
	CascadeClassifier cascade;//������������������  
	vector<Rect> rects;
	vector<Rect>::const_iterator pRect;

	double scale = 1.;
	Mat image;
	double t;
	if (!cascade.load(cascadeName))//��ָ�����ļ�Ŀ¼�м��ؼ���������  
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return 0;
	}

		image = imread("C:/souhudet/3.jpg");
		if (!image.empty())//��ȡͼƬ���ݲ���Ϊ��  
		{
			Mat gray, smallImg(cvRound(image.rows / scale), cvRound(image.cols / scale), CV_8UC1);//��ͼƬ��С���ӿ����ٶ�  
			cvtColor(image, gray, CV_BGR2GRAY);//��Ϊ�õ�����haar���������Զ��ǻ��ڻҶ�ͼ��ģ�����Ҫת���ɻҶ�ͼ��  
			resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);//���ߴ���С��1/scale,�����Բ�ֵ  
			equalizeHist(smallImg, smallImg);//ֱ��ͼ����  

											 //detectMultiScale������smallImg��ʾ����Ҫ��������ͼ��ΪsmallImg��rects��ʾ��⵽��Ŀ�����У�1.1��ʾ  
											 //ÿ��ͼ��ߴ��С�ı���Ϊ1.1��2��ʾÿһ��Ŀ������Ҫ����⵽3�β��������Ŀ��(��Ϊ��Χ�����غͲ�ͬ�Ĵ��ڴ�  
											 //С�����Լ�⵽Ŀ��),CV_HAAR_SCALE_IMAGE��ʾ�������ŷ���������⣬��������ͼ��Size(30, 30)ΪĿ���  
											 //��С���ߴ�  
			rects.clear();
			printf("begin...\n");
			t = (double)cvGetTickCount();//���������㷨ִ��ʱ��  
			cascade.detectMultiScale(smallImg, rects, 1.1, 2, 0, Size(140,65), Size(160, 75));
			//|CV_HAAR_FIND_BIGGEST_OBJECT//|CV_HAAR_DO_ROUGH_SEARCH|CV_HAAR_SCALE_IMAGE,  

			t = (double)cvGetTickCount() - t;
			printf("detection time = %g ms\n\n", t / ((double)cvGetTickFrequency()*1000.));
			for (pRect = rects.begin(); pRect != rects.end(); pRect++)
			{
				rectangle(image, cvPoint(pRect->x, pRect->y), cvPoint(pRect->x + pRect->width, pRect->y + pRect->height), cvScalar(0, 255, 0));
			}
			imwrite("C:/souhudet/rec.jpg", image);
		}
	}