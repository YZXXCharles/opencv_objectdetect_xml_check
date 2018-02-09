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

String cascadeName = "C:/souhudet/cascade.xml";//训练数据  

int main()
{
	CascadeClassifier cascade;//创建级联分类器对象  
	vector<Rect> rects;
	vector<Rect>::const_iterator pRect;

	double scale = 1.;
	Mat image;
	double t;
	if (!cascade.load(cascadeName))//从指定的文件目录中加载级联分类器  
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return 0;
	}

		image = imread("C:/souhudet/3.jpg");
		if (!image.empty())//读取图片数据不能为空  
		{
			Mat gray, smallImg(cvRound(image.rows / scale), cvRound(image.cols / scale), CV_8UC1);//将图片缩小，加快检测速度  
			cvtColor(image, gray, CV_BGR2GRAY);//因为用的是类haar特征，所以都是基于灰度图像的，这里要转换成灰度图像  
			resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);//将尺寸缩小到1/scale,用线性插值  
			equalizeHist(smallImg, smallImg);//直方图均衡  

											 //detectMultiScale函数中smallImg表示的是要检测的输入图像为smallImg，rects表示检测到的目标序列，1.1表示  
											 //每次图像尺寸减小的比例为1.1，2表示每一个目标至少要被检测到3次才算是真的目标(因为周围的像素和不同的窗口大  
											 //小都可以检测到目标),CV_HAAR_SCALE_IMAGE表示不是缩放分类器来检测，而是缩放图像，Size(30, 30)为目标的  
											 //最小最大尺寸  
			rects.clear();
			printf("begin...\n");
			t = (double)cvGetTickCount();//用来计算算法执行时间  
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