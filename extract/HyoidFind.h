#pragma once
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
#define MAXN 50
#define GAP 4
#define Swallow 31
class HyoidFind
{
public:
	HyoidFind();
	~HyoidFind();
	int cnt;
	typedef Point Vectors;
	#define eps (1e-8)
	#define PI 3.1415926
	static Point repel[20];
	static int repelRadius[20];
	static int repelNum;
	static int reset;
	static Point hyoidPoint;
	static Point CenterPoint;
	const double Dis1 = 66.1891;
	const double Dis2 = 121.742;
	const int blockSize = 3;
	int num[MAXN], val[MAXN], affirm[MAXN];
	static int dcmp(double x);
	static double Dot(const Vectors& A, const Vectors& B);
	static double Length(const Vectors& A);
	double Angle(const Vectors& A, const Vectors& B);
	double Cross(const Vectors& A, const Vectors& B);
	Point GetLineIntersection(const Point& P, const Point& v, const Point& Q, const Point& w);
	Vectors Rotate(const Vectors& A, double rad);
	bool SegmentProperIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2);
	bool OnSegment(const Point& p, const Point& a1, const Point& a2);
	bool _OnSegment(const Point& p, const Point& a1, const Point& a2);
	double DistanceToSegment(const Point& P, const Point& A, const Point& B);

	bool drawContour(int x, int y, Mat& image, const Mat& grad);
	Point run(const Mat& frame, VideoWriter writer,bool isGradient = true);
	friend Vectors operator + (const Vectors& A, const Vectors& B); 
	friend Vectors operator - (const Point& A, const Point& B);
	friend Vectors operator * (const Vectors& A, double p);
	friend Vectors operator / (const Vectors& A, double p);
	friend bool operator < (const Point& a, const Point& b);
	friend bool operator == (const Point& a, const Point &b);
	friend bool operator != (const Point& a, const Point &b);
	friend bool operator != (const Point& a, const Point &b);
};

