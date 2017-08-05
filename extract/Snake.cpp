#include "Snake.h"


Snake::Snake()
{
	NumOfClick = 0;
	dir[0] = 0, dir[1] = 1, dir[2] = -1, dir[3] = 2, dir[4] = -2, dir[5] = 3, dir[6] = -3;
}

int Snake::NumOfClick = 0;
Point Snake::region[ConstraintPoint + 10];
Point Snake::_region[ConstraintPoint + 10];
Point Snake::init[ConstraintPoint + 10];
Point Snake::palateTeeth[30];
Snake::~Snake()
{
}

void Snake::run(Mat& preframe, Mat& frame, Mat&image1, Mat&image2, int kase,VideoWriter writer)
{
	cvtColor(preframe, preI, COLOR_RGB2GRAY);
	cvtColor(frame, I, COLOR_RGB2GRAY);
	memset(dp, -0x3f, sizeof(dp));
	memset(pre, -0x3f, sizeof(pre));
	memset(brightArr, -0x3f, sizeof(brightArr));
	aveCircleE = 0;
	cvtColor(preI, image1, cv::COLOR_GRAY2BGR);
	cvtColor(I, image2, cv::COLOR_GRAY2BGR);
	if (!kase){
		imshow("image2", image2);
		//setMouseCallback("image2", mouseHandle, (void*)&image2);
		//waitKey(0);
		region[0] = Point(115, 115);
		region[1] = Point(116, 107);
		region[2] = Point(118, 98);
		region[3] = Point(122, 91);
		region[4] = Point(126, 84);
		region[5] = Point(132, 77);
		region[6] = Point(140, 72);
		region[7] = Point(150, 71);
		region[8] = Point(160, 71);
		region[9] = Point(168, 73);
		region[10] = Point(176, 78);
		region[11] = Point(182, 82);
		region[12] = Point(189, 87);
		region[13] = Point(195, 93);
		region[14] = Point(200, 99);
		setRegionOrInit(init, region);
		return;
	}
	else{
		setMouseCallback("image2", mouseHandle, (void*)&image2);
		//waitKey(0);
	}
	if (HyoidFind::reset == 1){
		//setRegionOrInit(region, init);
		//for (int i = 0; i < ConstraintPoint; i++)
			//circle(image2, init[i], 4, Scalar(0, 255, 255));
		cout << " reset " << endl;
		//return;
	}

	DynamicProgramming(image1, image2);
	DrawCircleAndUpdate(image1, image2);
	//cout << aveCircleE << " maxn circle " << endl;
	
	//line(image2, Point(beginx, beginy), Point(beginx, endy), Scalar(255, 0, 0));
	//line(image2, Point(endx, beginy), Point(endx, endy), Scalar(255, 0, 0));
	//line(image1, Point(170, 75), Point(190, 75), Scalar(255, 0, 255));
	//line(image1, Point(242, 110), Point(242, 130), Scalar(255, 0, 255));
	//line(image2, Point(170, 75), Point(190, 75), Scalar(255, 0, 255));
	//line(image2, Point(242, 110), Point(242, 130), Scalar(255, 0, 255));
	imshow("image1", image1);
	imshow("image2", image2);
	waitKey(1);
}

void Snake::mouseHandle(int myevent, int x, int y, int flags, void* param)
{
	Mat& image = *(Mat*)param;
	if (myevent == EVENT_LBUTTONDOWN)
	{
		if (NumOfClick >= ConstraintPoint)
		{
			cout << Point(x, y) << endl;
			circle(image, Point(x, y), 1, Scalar(0, 255, 255));
			imshow("image2", image);
		}
		else
		{
			region[NumOfClick] = Point(x, y);
			int deltaY = 1;
			if(NumOfClick)deltaY = region[NumOfClick].y - region[NumOfClick - 1].y;
			int deltaX = 1;
			if (NumOfClick)deltaX = region[NumOfClick].x - region[NumOfClick - 1].x;
			double degree = 0;
			if (NumOfClick)degree = atan2(deltaY, deltaX) * 180 / PI;
			cout << degree << " degree " << endl;
			if (NumOfClick == 0 || limitDis(region[NumOfClick], region[NumOfClick - 1]))
			{
				NumOfClick++;
				circle(image, Point(x, y), 3, Scalar(0, 255, 255));
			}
			imshow("image2", image);
		}
		
	}
}
void Snake::DrawCircleAndUpdate(Mat& image1, Mat& image2)
{
	int ind = -1, pind = -1, maxn = 0;
	for (int i = 0; i < searchSize; i++)for (int j = 0; j < searchSize; j++)
	{
		for (int pi = 0; pi < searchSize; pi++)for (int pj = 0; pj < searchSize; pj++)
		{
			//cout << dp[1][i * searchSize + j][pi * searchSize + pj] << " ";
			if (maxn < dp[ConstraintPoint - 1][i * searchSize + j][pi * searchSize + pj])
			{
				maxn = dp[ConstraintPoint - 1][i * searchSize + j][pi * searchSize + pj];
				ind = i * searchSize + j;
				pind = pi * searchSize + pj;
			}
		}
	}
	//cout << maxn << " maxnbright " << endl;
	int i = ind / searchSize;
	int j = ind % searchSize;
	int x = region[ConstraintPoint - 1].x + dir[i];
	int y = region[ConstraintPoint - 1].y + dir[j];
	circle(image1, region[ConstraintPoint - 1], 4, Scalar(0, 255, 255));
	region[ConstraintPoint - 1] = Point(x, y);
	circle(image2, Point(x, y), 4, Scalar(0, 255, 255));
	vector<int>vec1, vec2;
	for (int k = ConstraintPoint - 2; k >= 0; k--)
	{
		int i = pind / searchSize;
		int j = pind % searchSize;
		int x = region[k].x + dir[i];
		int y = region[k].y + dir[j];
		circle(image1, region[k], 4, Scalar(0, 255, 255));
		region[k] = Point(x, y);
		circle(image2, Point(x, y), 4, Scalar(0, 255, 255));
		vec1.push_back(ind);
		vec2.push_back(pind);
		int ppind = pre[k + 1][ind][pind];
		ind = pind;
		pind = ppind;
		if ((ind < 0 || pind < 0) && k)
		{
			for (int i = 0; i < vec1.size(); i++)
			{
				cout << ConstraintPoint - 1 - i << " " << dp[ConstraintPoint - 1 - i][vec1[i]][vec2[i]] << endl;
				cout << " pind " << vec2[i] << endl;
				cout << " ind " << vec1[i] << endl;
			}
			cout << k << " k " << endl;
			cout << " waaaaaaaa " << endl;
			getchar();
			setRegionOrInit(region, init);
			return;
		}
	}
	for (int i = 2; i < ConstraintPoint; i++)
	{
		for (int j = 1; i - j * 2 >= 0; j++)
		{
			double dis1 = dis(region[i], region[i - j]);
			double dis2 = dis(region[i - j], region[i - 2 * j]);
			double dis3 = dis(region[i], region[i - 2 * j]);
			if (dis3 < dis1 || dis3 < dis2)
			{
				for (int i = 0; i < ConstraintPoint; i++)
				{
					region[i] = init[i];
					circle(image2, region[i], 4, Scalar(0, 255, 255));
				}
				cout << " triangle reset " << endl;
				return;
			}
		}
	}
}
void Snake::DynamicProgramming(Mat& image1, Mat& image2)
{
	bool vis[10];
	memset(vis, 0, sizeof(vis));
	for (int k = 0; k < ConstraintPoint; k++)
	{
#pragma omp parallel for
		for (int i = 0; i < searchSize; i++)for (int j = 0; j < searchSize; j++)
		{
			int x = region[k].x + dir[i];
			int y = region[k].y + dir[j];
			if (brightArr[y][x] < 0)
			{
				int bright = 0;
				for (int x0 = x - blockSize / 2; x0 <= x + blockSize / 2; x0++)
					for (int y0 = y - blockSize / 2; y0 <= y + blockSize / 2; y0++)
						bright += (image2.at<Vec3b>(y0, x0)[0] + image2.at<Vec3b>(y0, x0)[1] + image2.at<Vec3b>(y0, x0)[2]);
				brightArr[y][x] = bright / (blockSize * blockSize);
			}
			if (k == 0){}
			else if (k == 1)for (int pi = 0; pi < searchSize; pi++)for (int pj = 0; pj < searchSize; pj++)
			{
				int px = region[k - 1].x + dir[pi];
				int py = region[k - 1].y + dir[pj];
				double internalE = 0;
				if (atan2(y - py, x - px) * 180 / PI < 0)internalE -= atan2(y - py, x - px) * 180 / PI / 8;
				else internalE -= 100;
				if (limitDis(Point(px, py), Point(x, y)) && limitX(Point(px, py), Point(x, y))/* &&
					hyoidRepel(Point(x, y)) && hyoidRepel(Point(px, py))*/)
				{
					//printf("(%d %d) (%d %d)\n", px, py, x, y);
					double cicleE = dis(HyoidFind::CenterPoint, Point(x, y));
					aveCircleE = max(aveCircleE, (int)cicleE);
					if (cicleE > 144)cicleE = -cicleE;
					if (dp[k][i * searchSize + j][pi * searchSize + pj] < brightArr[y][x] + brightArr[py][px] + internalE + cicleE)
					{
						pre[k][i * searchSize + j][pi * searchSize + pj] = 0;
						dp[k][i * searchSize + j][pi * searchSize + pj] = brightArr[y][x] + brightArr[py][px] + internalE + cicleE;
					}
				}
			}
			else if (k >= 2)for (int pi = 0; pi < searchSize; pi++)for (int pj = 0; pj < searchSize; pj++)
			{
				int px = region[k - 1].x + dir[pi];
				int py = region[k - 1].y + dir[pj];
				for (int ppi = 0; ppi < searchSize; ppi++)for (int ppj = 0; ppj < searchSize; ppj++)
				{
					int ppx = region[k - 2].x + dir[ppi];
					int ppy = region[k - 2].y + dir[ppj];
					if (limitDis(Point(ppx, ppy), Point(px, py)) && limitDis(Point(px, py), Point(x, y)) &&
						repulsive(Point(ppx, ppy), Point(x, y), 2) && limitX(Point(ppx, ppy), Point(px, py)) &&
						limitX(Point(px, py), Point(x, y))
						/*&& hyoidRepel(Point(x, y)) &&*/
						/*hyoidDerivative(HyoidFind::hyoidPoint, Point(ppx, ppy), Point(px, py), Point(x, y))*/)
					{

						if (fabs(atan2(y - py, x - px) * 180 / PI - atan2(py - ppy, px - ppx) * 180 / PI) > 80)continue;
						//limit degree 
						//if (!(atan2(y - py, x - px) * 180 / PI + 5 >= atan2(py - ppy, px - ppx) * 180 / PI))continue;
						if ((y - py) * (px - ppx) <= 0.90 * (py - ppy) * (x - px))continue;
						//quadratic limitation
						/*if (!vis[k])
						{
						cout << " whether or not " << endl;
						vis[k] = 1;
						}*/
						double cicleE = dis(HyoidFind::CenterPoint, Point(x, y));
						aveCircleE = max(aveCircleE, (int)cicleE);
						if (cicleE > 144)cicleE = -cicleE;
						int internalE = -fabs(atan2(y - py, x - px) * 180 / PI - atan2(py - ppy, px - ppx) * 180 / PI) / 6;
						if (k < ConstraintPoint / 2 && (y - py) > 0.2){
							//continue;
							//if (atan2(y - py, x - px) * 180 / PI < 0)
							//	internalE -= atan2(y - py, x - px) * 180 / PI / 8;
							//else
							internalE -= 100;
						}
						else if (k > ConstraintPoint / 2 && (y - py) < -0.2){
							//continue;
							//if (atan2(y - py, x - px) * 180 / PI > 0)
							//	internalE += 0;
							//else
							internalE -= 100;
						}
						//cout << " you " << endl;
						if (brightArr[y][x] < 0)cout << " brightArr[y][x] < 0 " << endl;
						int externalE = dp[k - 1][pi * searchSize + pj][ppi * searchSize + ppj] + brightArr[y][x];
						if (dp[k][i * searchSize + j][pi * searchSize + pj] < externalE + internalE + cicleE)
						{
							pre[k][i * searchSize + j][pi * searchSize + pj] = ppi * searchSize + ppj;
							dp[k][i * searchSize + j][pi * searchSize + pj] = externalE + internalE + cicleE;
						}
					}
				}
			}

		}
	}
}
double Snake::dis(int x1, int y1, int x2, int y2){
	return sqrt((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
double Snake::dis(Point p1, Point p2){
	return dis(p1.x, p1.y, p2.x, p2.y);
}
bool Snake::limitDis(Point p1, Point p2, int num, int len, int delta){
	double cur = dis(p1, p2);
	return cur >= len * num - delta * num && cur <= len * num + delta * num;
}
bool Snake::repulsive(Point p1, Point p2, int num, int len, int delta){
	return dis(p1, p2) >= len * num - delta * num;
}
bool Snake::limitX(Point p1, Point p2, int delta){
	return p2.x - p1.x > delta;
}
bool Snake::hyoidRepel(Point p){
	for (int i = 0; i < HyoidFind::repelNum; i++){
		if (HyoidFind::Length(HyoidFind::repel[i] - p) <= HyoidFind::repelRadius[i])
			return false;
	}
	return true;
}
bool Snake::hyoidDerivative(Point p0,Point p1,Point p2,Point p3){
	double d1 = (double)(p1.x - p0.x) / (p0.y - p1.y);
	double d2 = (double)(p2.x - p0.x) / (p0.y - p2.y);
	double d3 = (double)(p3.x - p0.x) / (p0.y - p3.y);
	return d1 < d2 && d2 < d3;
}
void Snake::setRegionOrInit(Point *a, const Point *b, int num){
	for (int i = 0; i < num; i++)
		a[i] = b[i];
}