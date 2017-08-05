#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <opencv2/video/background_segm.hpp>
#include <algorithm>
#include <vector>
using namespace cv;
using namespace std;
const int beginx = 120, beginy = 40, endx = 180, endy = 100;
//const int beginx = 120, beginy = 60, endx = 170, endy = 90;
const int Y = endy - beginy + 10;
const int X = endx - beginx + 10;
const int blockSize = 15, searchSize = 5, shiftSize = 5;
Mat I, preI, frame, preframe, blank;
#define GRADIENT 1
#define DEBUGPOINT 0
int last[4];
int dy[100000][4];
int main()
{
	string fileName = "gradient.avi";
	VideoCapture cap(fileName); // open the default camera
	if (!cap.isOpened()) return -1; // check if we succeeded
	for (int kase = 0;; kase++)
	{
		cap >> frame; // get a new frame from camera
		cvtColor(frame, I, COLOR_RGB2GRAY);
		if (fileName[0] == '1')GaussianBlur(preI, preI, Size(5, 5), 0);
		if (fileName[0] == '1')GaussianBlur(I, I, Size(5, 5), 0);
		//threshold(preI, preI, 70, 255, THRESH_BINARY);
		//threshold(I, I, 70, 255, THRESH_BINARY);
		threshold(preI, preI, 70, 255, THRESH_TOZERO);
		threshold(I, I, 70, 255, THRESH_TOZERO);
		//medianBlur(preI, preI, 5);
		//medianBlur(I, I, 5);
		//erode(preI, preI, Mat());
		//erode(I, I, Mat());
		//dilate(preI, preI, Mat());
		//dilate(I, I, Mat());

		//adaptiveThreshold(I, I, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 10);
		//adaptiveThreshold(preI, preI, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 10);
		int ind[4];
		//cout << frame.channels() << endl; ///why channels == 3
		int segment = (endx - beginx) / 4;
		for (int x = beginx, i = 0; i < 4; x += segment, i++)
		{
			int maxn = 0;
			uchar * inPtr = I.data + I.channels() * x + beginy*I.step;
			for (int y = beginy; y <= endy; y++, inPtr += I.step)
			{
				if ((*inPtr) > maxn)
				{
					maxn = *inPtr;
					ind[i] = y;
				}
			}
		}
		if (kase)
		{
			for (int i = 0; i < 4; i++)dy[kase - 1][i] = abs(last[i] - ind[i]);
			memcpy(last, ind, sizeof(ind));
			for (int i = 0; i < 4; i++)cout << dy[kase - 1][i] << " ";
			cout << endl;
		}
		else memcpy(last, ind, sizeof(ind));
		for (int x = beginx, i = 0; i < 4; x += segment, i++)
		{
			circle(I, Point(x, ind[i]), 1, Scalar(255));
		}
		line(I, Point(beginx, beginy), Point(beginx, endy), Scalar(255));
		line(I, Point(endx, beginy), Point(endx, endy), Scalar(255));
		imshow("circle", I);
		//imshow("waiter", wait);
		waitKey(100);
		swap(frame, preframe);
	}

}