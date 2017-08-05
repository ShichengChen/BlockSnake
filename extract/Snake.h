#pragma once
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <opencv2/video/background_segm.hpp>
#include <algorithm>
#include <vector>
#include "HyoidFind.h"
#define Swallow 31

using namespace cv;
using namespace std;

class Snake
{
public:
	Snake();
	~Snake();
	#define PI 3.14159265
	static const int ConstraintPoint = 15;
	static const int blockSize = 9, searchSize = 7, shiftSize = 7;
	static const int beginx = 100 - 20, beginy = 50 - 20, endx = 200 + 20, endy = 120 + 20;
	int dir[searchSize];
	int aveCircleE;
	static int NumOfClick;
	int dp[ConstraintPoint][searchSize * searchSize][searchSize * searchSize];
	int pre[ConstraintPoint][searchSize * searchSize][searchSize * searchSize];
	int brightArr[400][400];
	static Point region[ConstraintPoint + 10];
	static Point _region[ConstraintPoint + 10];
	static Point init[ConstraintPoint + 10];
	static Point palateTeeth[30];
	int maxnInter;
	int maxnBri;
	Mat I, preI, frame, preframe, blank;
	static double dis(int x1, int y1, int x2, int y2);
	static double dis(Point p1, Point p2);
	static bool limitDis(Point p1, Point p2, int num = 1, int len = blockSize, int delta = 2);
	bool repulsive(Point p1, Point p2, int num = 1, int len = blockSize, int delta = 2);
	bool limitX(Point p1, Point p2, int delta = 2);
	static void mouseHandle(int myevent, int x, int y, int flags, void* param);
	void DynamicProgramming(Mat& image1, Mat& image2);
	void DrawCircleAndUpdate(Mat& image1, Mat& image2);
	void run(Mat& preframe, Mat& frame, Mat&image1, Mat&image2, int kase, VideoWriter writer);
	bool hyoidRepel(Point p);
	bool hyoidDerivative(Point p0, Point p1, Point p2, Point p3);
	void setRegionOrInit(Point *a, const Point *b, int num = ConstraintPoint);
};

