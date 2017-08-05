#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <opencv2/video/background_segm.hpp>
#include <algorithm>
#include <vector>
#include "Snake.h"
#include "HyoidFind.h"
#include <Windows.h>
#include <ctime>
#include <cstdlib>
LARGE_INTEGER Freq;
LARGE_INTEGER Begin;
LARGE_INTEGER End;
#define Swallow 31
using namespace cv;
using namespace std;
int main()
{
	VideoCapture cap("1.avi"); // open the default camera
	if (!cap.isOpened()) return -1; // check if we succeeded
	Mat preframe, frame;
	//136-1700
	//1700-3633
	for (int i = 0; i < 136;i++)cap >> frame;

	cap >> preframe;

	VideoWriter writer("tkBlockSnake.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(320, 240), 4);
	VideoWriter writer1("hyoidfind.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(320, 240), 4);
	Mat g = imread("g.png");
	cout << g.cols << " col rows " << g.rows << endl;
	//53 col rows 47
	Mat t = imread("t.png");
	cout << t.cols << " col rows " << t.rows << endl;
	//41 col rows 44

	Mat escalator = imread("escalator.png");
	cout << escalator.cols << " col rows " << escalator.rows << endl;
	//88 col rows 30


	cout << cap.get(CV_CAP_PROP_FPS) << endl;
	Snake sk;
	HyoidFind hf;
	Mat image1, image2;
	srand(time(NULL));
	for (int kase = 0,cnt = 0;; kase++)
	{
		cap >> frame;
		if (frame.data == NULL)break;
		if (kase < 0)
		{
			swap(frame, preframe);
			continue;
		}
		QueryPerformanceFrequency(&Freq);
		QueryPerformanceCounter(&Begin);
		printf("kase %d\n", kase);
		
		
		//[178, 63] [179, 66] [182, 71] [178, 72] [180, 71]
		//[251, 119] [247, 118] [246, 119] [251, 119]
		hf.run(frame, writer1,true);
		sk.run(preframe, frame, image1, image2, kase - 0, writer);
		if(kase < 1700 - 136)
			addWeighted(image2(Rect(0, 150, 53, 47)), 0.5, g(Rect(0,0,53,47)), 0.5, 0, image2(Rect(0, 150, 53, 47)));
		//53 col rows 47
		//136-1700
		//1700-3633
		if (kase >= 1700 - 136 && kase < 3633 - 136)
			addWeighted(image2(Rect(0, 150, 41, 44)), 0.5, g(Rect(0, 0, 41, 44)), 0.5, 0, image2(Rect(0, 150, 41, 44)));
		//41 col rows 44
		if (kase >= 3633 - 136)
			addWeighted(image2(Rect(0, 150, 88, 30)), 0.5, g(Rect(0, 0, 88, 30)), 0.5, 0, image2(Rect(0, 150, 88, 30)));
		//88 col rows 30
		imshow("image2", image2);
		/*
		Mat src_gray, grad_x, abs_grad_x, grad_y, abs_grad_y, grad;
		cvtColor(frame, src_gray, CV_RGB2GRAY);
		Sobel(src_gray, grad_x, CV_16S, 1, 0, 3);
		convertScaleAbs(grad_x, abs_grad_x);
		Sobel(src_gray, grad_y, CV_16S, 0, 1, 3);
		convertScaleAbs(grad_y, abs_grad_y);
		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
		
		Mat gray;
		cvtColor(frame, gray, CV_BGR2GRAY);
		for (int i = 0; i < Snake::ConstraintPoint;i++){
			char buffer[20];
			int r = Snake::blockSize;
			sprintf_s(buffer, "region/%d.jpg", kase * Snake::ConstraintPoint * 2 + (i << 1 | 0));
			Point a = Point(Snake::region[i].x - 1.5 * r, Snake::region[i].y - 1.5 * r);
			Point b = Point(Snake::region[i].x + 1.5 * r, Snake::region[i].y + 1.5 * r);
			imwrite(buffer, gray(Rect(a, b)));
			sprintf_s(buffer, "region/%d.jpg", kase * Snake::ConstraintPoint * 2 + (i << 1 | 1));
			imwrite(buffer, grad(Rect(a, b)));
		}
		if (cnt <= kase * Snake::ConstraintPoint * 2)
		{
			for (int x = Snake::blockSize * 1.5; x < gray.rows - 1.5 * Snake::blockSize; x += 3)
				for (int y = Snake::blockSize * 1.5; y < gray.rows - 1.5 * Snake::blockSize; y += 3)
			{
				int r = Snake::blockSize;
				bool suc = true;
				for (int j = 0; j < Snake::ConstraintPoint; j++){
					if (Snake::dis(Snake::region[j], Point(x, y)) > 1.5 * r);
					else suc = false;
				}
				if (suc)
				{
					char buffer[20];
					sprintf_s(buffer, "_region/%d.jpg", cnt++);
					imwrite(buffer, gray(Rect(Point(x - 1.5 * r, y - 1.5 * r), Point(x + 1.5 * r, y + 1.5 * r))));
					sprintf_s(buffer, "_region/%d.jpg", cnt++);
					imwrite(buffer, grad(Rect(Point(x - 1.5 * r, y - 1.5 * r), Point(x + 1.5 * r, y + 1.5 * r))));
					//cout << cnt << " cnt " << endl;
				}
			}
		}
		*/

		swap(frame, preframe);
		QueryPerformanceCounter(&End);
		double time = (double)(End.QuadPart - Begin.QuadPart) / Freq.QuadPart;
		printf("time %f\n", time);
		printf("%d\n", kase);
	}
	writer.release();
	writer1.release();
}
