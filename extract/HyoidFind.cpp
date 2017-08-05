#include "HyoidFind.h"


HyoidFind::HyoidFind()
{
	reset = 0;
	CenterPoint = Point(158, 200);
}


HyoidFind::~HyoidFind()
{
}

int HyoidFind::dcmp(double x) {
	if (fabs(x) < eps) return 0; 
	return x < 0 ? -1 : 1;
}
Point HyoidFind::repel[20];
Point HyoidFind::CenterPoint;
int HyoidFind::repelRadius[20];
int HyoidFind::repelNum;
int HyoidFind::reset;
Point HyoidFind::hyoidPoint;

double HyoidFind::Dot(const Vectors& A, const Vectors& B) { return A.x*B.x + A.y*B.y; }
double HyoidFind::Length(const Vectors& A) { return sqrt(Dot(A, A)); }
double HyoidFind::Angle(const Vectors& A, const Vectors& B) { return acos(Dot(A, B) / Length(A) / Length(B)); }
double HyoidFind::Cross(const Vectors& A, const Vectors& B) { return A.x*B.y - A.y*B.x; }
Point HyoidFind::GetLineIntersection(const Point& P, const Point& v, const Point& Q, const Point& w) {
	Vectors u = P - Q;
	double t = Cross(w, u) / Cross(v, w);
	return P + v*t;
}
HyoidFind::Vectors HyoidFind::Rotate(const HyoidFind::Vectors& A, double rad) {
	return Vectors(A.x*cos(rad) - A.y*sin(rad), A.x*sin(rad) + A.y*cos(rad));
}
bool HyoidFind::SegmentProperIntersection(const Point& a1, const Point& a2, const Point& b1, const Point& b2) {
	double c1 = Cross(a2 - a1, b1 - a1), c2 = Cross(a2 - a1, b2 - a1),
		c3 = Cross(b2 - b1, a1 - b1), c4 = Cross(b2 - b1, a2 - b1);
	return dcmp(c1)*dcmp(c2)<0 && dcmp(c3)*dcmp(c4)<0;
}
bool HyoidFind::OnSegment(const Point& p, const Point& a1, const Point& a2) {
	return dcmp(Cross(a1 - p, a2 - p)) == 0 && dcmp(Dot(a1 - p, a2 - p)) < 0;
}
bool HyoidFind::_OnSegment(const Point& p, const Point& a1, const Point& a2) {
	return dcmp(Cross(a1 - p, a2 - p)) == 0 && dcmp(Dot(a1 - p, a2 - p)) < 0
		&& p != a1 && p != a2;
}
double HyoidFind::DistanceToSegment(const Point& P, const Point& A, const Point& B) {
	if (A == B) return Length(P - A);
	Vectors v1 = B - A, v2 = P - A, v3 = P - B;
	if (dcmp(Dot(v1, v2)) < 0) return Length(v2);
	else if (dcmp(Dot(v1, v3)) > 0) return Length(v3);
	else return fabs(Cross(v1, v2)) / Length(v1);
}
bool HyoidFind::drawContour(int x, int y, Mat& image, const Mat& grad)
{
	int upper = 0, lower1 = 0, lower2 = 0, lower3 = 0, topright = 0, top = 0;
	for (int x1 = x - blockSize / 2; x1 <= x + blockSize / 2; x1++)
		for (int y1 = y - blockSize / 2; y1 <= y + blockSize / 2; y1++)
			upper += grad.at<uchar>(y1, x1);
	for (int x1 = x - blockSize / 2; x1 <= x + blockSize / 2; x1++)
		for (int y1 = (y + blockSize) - blockSize / 2; y1 <= (y + blockSize) + blockSize / 2; y1++)
			lower1 += grad.at<uchar>(y1, x1);
	for (int x1 = (x + blockSize) - blockSize / 2; x1 <= (x + blockSize) + blockSize / 2; x1++)
		for (int y1 = (y + blockSize) - blockSize / 2; y1 <= (y + blockSize) + blockSize / 2; y1++)
			lower2 += grad.at<uchar>(y1, x1);
	for (int x1 = (x + blockSize) - blockSize / 2; x1 <= (x + blockSize) + blockSize / 2; x1++)
		for (int y1 = y - blockSize / 2; y1 <= y + blockSize / 2; y1++)
			lower3 += grad.at<uchar>(y1, x1);
	for (int x1 = (x + blockSize) - blockSize / 2; x1 <= (x + blockSize) + blockSize / 2; x1++)
		for (int y1 = (y + blockSize) - blockSize / 2; y1 <= (y + blockSize) + blockSize / 2; y1++)
			topright += grad.at<uchar>(y1, x1);
	for (int x1 = x - blockSize / 2; x1 <= x + blockSize / 2; x1++)
		for (int y1 = (y + blockSize) - blockSize / 2; y1 <= (y + blockSize) + blockSize / 2; y1++)
			top += grad.at<uchar>(y1, x1);
	//if (upper - lower > 10)circle(image, Point(x, y), 3, Scalar(255, 0, 0));
	return upper - lower1 > 0 || upper - lower2 > 0 || upper - lower3 > 0 || upper - topright > 0 || upper - top > 0;
}
Point HyoidFind::run(const Mat& frame, VideoWriter writer,bool isGradient)
{
	Mat image, src_gray, grad_x, abs_grad_x, grad_y, abs_grad_y, grad, cur;;
	frame.copyTo(image);
	GaussianBlur(image, image, Size(5, 5), 1, 0);
	if (isGradient)
	{
		cvtColor(image, src_gray, CV_RGB2GRAY);
		Sobel(src_gray, grad_x, CV_16S, 1, 0, 3);
		convertScaleAbs(grad_x, abs_grad_x);
		Sobel(src_gray, grad_y, CV_16S, 0, 1, 3);
		convertScaleAbs(grad_y, abs_grad_y);
		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	}
	else
	{
		grad = image;
	}
	//medianBlur(frame, frame, 5);


	memset(affirm, 0, sizeof(affirm));
	memset(num, 0, sizeof(num));
	memset(val, 0, sizeof(val));
	for (int i = 0; i < MAXN; i++)num[i] = 1;
	//for (int i = 0; i < 18; i++)cout << val[i] << endl;
	//cout << CenterPoint.x << "  x y " << CenterPoint.y << endl;
	for (int x = 0; x < image.cols; x++)
	{
		for (int y = 0; y < min(image.rows, CenterPoint.y - 1); y++)
		{
			double len = Length(Point(x, y) - CenterPoint);
			if (len >= Dis2 || len <= Dis1)continue;
			double degrees = atan2(CenterPoint.y - y, x - CenterPoint.x) * 180 / PI;
			//circle(image, Point(x, y), 1, Scalar(255));
			//if (fabs(degrees / GAP-(int)degrees / GAP) < 0.1)line(image, Point(x, y), CenterPoint, Scalar(255,255,255));
			//cout << degrees << " ";
			val[(int)degrees / GAP] += (image.at<Vec3b>(y, x)[0] + image.at<Vec3b>(y, x)[1] + image.at<Vec3b>(y, x)[2]) / 3;
			num[(int)degrees / GAP] ++;
		}
	}
	int minn = 100, ind;
	for (int i = 0; i < MAXN; i++)
	{
		val[i] /= num[i];
		if (i > 22 && i < 40)
		{
			if (minn > val[i])minn = val[i], ind = i;
		}
	}
	if (ind <= Swallow)reset = 1;
	else reset = 0;
	int ymaxn = 0, xmaxn = 0, powdis = 100000000;
	for (int x = 0; x < frame.cols; x++)
	{
		for (int y = 0; y < min(frame.rows, CenterPoint.y - 1); y++)
		{
			double len = Length(Point(x, y) - CenterPoint);
			if (len >= Dis2 || len <= Dis1)continue;
			double degrees = atan2(CenterPoint.y - y, x - CenterPoint.x) * 180 / PI;
			int curi = (int)degrees / GAP;
			if (curi != ind && curi != ind - 1 && curi != ind + 1)continue;
			if (drawContour(x, y, image, grad))if (pow(abs(CenterPoint.y - y), 2.0) + pow(abs(CenterPoint.x - x), 2.0) < powdis)
			{
				xmaxn = x;
				ymaxn = y;
				powdis = pow(abs(CenterPoint.y - y), 2.0) + pow(abs(CenterPoint.x - x), 2.0);
			}
		}
	}
	ymaxn = ymaxn + 20;
	hyoidPoint = Point(xmaxn, ymaxn);
	circle(image, Point(xmaxn, ymaxn), 3, Scalar(255, 0, 0));
	circle(image, Point(xmaxn, ymaxn), 20, Scalar(255, 0, 0));
	circle(image, Point(xmaxn + (CenterPoint.x - xmaxn) / 3, ymaxn + (CenterPoint.y - ymaxn) / 3), 30, Scalar(255, 0, 0));
	circle(image, CenterPoint, 80, Scalar(255, 0, 0));
	circle(image, Point(CenterPoint.x, CenterPoint.y - 30), 60, Scalar(255, 0, 0));
	repelNum = 4;
	repel[0] = Point(xmaxn, ymaxn); 
	repel[1] = Point(xmaxn + (CenterPoint.x - xmaxn) / 3, ymaxn + (CenterPoint.y - ymaxn) / 3);
	repel[2] = CenterPoint; 
	repel[3] = Point(CenterPoint.x, CenterPoint.y - 30);
	repelRadius[0] = 20; 
	repelRadius[1] = 30; 
	repelRadius[2] = 84; 
	repelRadius[3] = 64;
	imshow("image", image);
	waitKey(1);
	return Point(xmaxn, ymaxn);
}
HyoidFind::Vectors operator + (const HyoidFind::Vectors& A, const HyoidFind::Vectors& B) { return HyoidFind::Vectors(A.x + B.x, A.y + B.y); }
HyoidFind::Vectors operator - (const Point& A, const Point& B) { return HyoidFind::Vectors(A.x - B.x, A.y - B.y); }
HyoidFind::Vectors operator * (const HyoidFind::Vectors& A, double p) { return HyoidFind::Vectors(A.x*p, A.y*p); }
HyoidFind::Vectors operator / (const HyoidFind::Vectors& A, double p) { return HyoidFind::Vectors(A.x / p, A.y / p); }
bool operator < (const Point& a, const Point& b) {
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}
bool operator == (const Point& a, const Point &b) {
	return HyoidFind::dcmp(a.x - b.x) == 0 && HyoidFind::dcmp(a.y - b.y) == 0;
}
bool operator != (const Point& a, const Point &b) {
	return !(a == b);
}