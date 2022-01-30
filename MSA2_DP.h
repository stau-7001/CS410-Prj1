#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;
vector<vector<int>> points(200),anspoints(200);
const int Mismatch = 3, gap = 2;
inline int alpha(char c1, char c2) {
	if (c1 == c2) return 0;
	return 3;
}

int editDistanceDP(string& X, string& Y, const int& delta) {
	int m = X.length();
	int n = Y.length();
	vector<vector<int>> dp(m + 1);
	for (int i = 0; i <= m; ++i) {
		dp[i].resize(n + 1);
	}
	for (int j = 0; j <= n; ++j) {
		dp[0][j] = j > 0 ? dp[0][j - 1] + delta : 0;
	}
	for (int i = 1; i <= m; ++i) {
		dp[i][0] = dp[i - 1][0] + delta;
		for (int j = 1; j <= n; ++j) {
			dp[i][j] = dp[i - 1][j - 1] + alpha(X[i - 1], Y[j - 1]);
			dp[i][j] = min(dp[i][j], dp[i - 1][j] + delta);
			dp[i][j] = min(dp[i][j], dp[i][j - 1] + delta);
		}
	}
	return dp[m][n];
}

void findpath(string& X, string& Y, const int& delta)
{
	int m = X.length();
	int n = Y.length();
	vector<vector<int>> pre(m + 1);
	vector<vector<int>> dp(m + 1);
	for (int i = 0; i <= m; ++i) {
		dp[i].resize(n + 1);
		pre[i].resize(n + 1);
	}
	for (int j = 0; j <= n; ++j) {
		dp[0][j] = j > 0 ? dp[0][j - 1] + delta : 0;
		pre[0][j] = j > 0 ? -1: -2;
	}
	for (int i = 1; i <= m; ++i) {
		dp[i][0] = dp[i - 1][0] + delta;
		pre[i][0] = 1;
		for (int j = 1; j <= n; ++j) {
			dp[i][j] = dp[i - 1][j - 1] + alpha(X[i - 1], Y[j - 1]);//Æ¥Åä
			dp[i][j] = min(dp[i][j], dp[i - 1][j] + delta);
			dp[i][j] = min(dp[i][j], dp[i][j - 1] + delta);
			if (dp[i][j] == dp[i - 1][j - 1] + alpha(X[i - 1], Y[j - 1]))
				pre[i][j] = 0;
			else if (dp[i][j] == dp[i - 1][j] + delta)
				pre[i][j] = 1;
			else pre[i][j] = -1;
		}
	}
	int i = m, j = n;
	string ansX, ansY;
	while (i != 0 || j != 0)
	{
		if (pre[i][j] == 1)
		{
			ansX = X[i - 1] + ansX;
			ansY = '-' + ansY;
			--i;
		}
		else if (pre[i][j] == 0)
		{
			ansX = X[i - 1] + ansX;
			ansY = Y[j - 1] + ansY;
			--i;
			--j;
		}
		else {
			ansX = '-' + ansX;
			ansY = Y[j - 1] + ansY;
			--j;
		}
	}
	cout << "Alignment:" << endl;
	cout << ansX;
	cout << ansY;
}

int editDistanceDP(string& X, string& Y, int xLow, int yLow, int xHigh, int yHigh, const int& delta, bool reverse, vector<int>& dist) {
	if (!reverse) {
		int m = xHigh - xLow;
		int n = yHigh - yLow;
		int la = 0, la2;
		dist.resize(m + 1);
		for (int i = 0; i <= m; ++i) {
			dist[i] = i > 0 ? dist[i - 1] + delta : 0;
		}
		for (int j = 1; j <= n; ++j)
		{
			for (int i = 0; i <= m; ++i)
			{
				la2 = dist[i];
				if (i == 0) {
					dist[i] = dist[i] + delta;
				}
				else {
					dist[i] = dist[i] + delta;//ºá±ß
					dist[i] = min(dist[i], la + alpha(X[xLow + i - 1], Y[yLow + j - 1]));//Ð±±ß
					dist[i] = min(dist[i], dist[i - 1] + delta);//Êú±ß
				}
				la = la2;
			}
		}

	}
	else {
		int m = xHigh - xLow;
		int n = yHigh - yLow;
		int la = 0, la2;
		dist.resize(m + 1);
		for (int i = 0; i <= m; ++i) {
			dist[i] = i > 0 ? dist[i - 1] + delta : 0;
		}
		for (int j = 1; j <= n; ++j)
		{
			for (int i = 0; i <= m; ++i)
			{
				la2 = dist[i];
				if (i == 0) {
					dist[i] = dist[i] + delta;
				}
				else {
					dist[i] = dist[i] + delta;//ºá±ß
					dist[i] = min(dist[i], la + alpha(X[xHigh - i], Y[yHigh - j]));//Ð±±ß
					dist[i] = min(dist[i], dist[i - 1] + delta);//Êú±ß
				}
				la = la2;
			}
		}
	}
	return 0;
}

int editDistanceDC(string& X, string& Y, int xLow, int yLow, int xHigh, int yHigh, const int& delta) {
	if (yHigh - yLow <= 1) {
		vector<int> dist;
		editDistanceDP(X, Y, xLow, yLow, xHigh, yHigh, delta, false, dist);
		int QMin = xLow;
		int Min = dist[0] + (xHigh - xLow) * delta;
		for (int q = 1; q <= xHigh - xLow; ++q) {
			int tmp = dist[q] + (xHigh - xLow - q) * delta;
			if (tmp < Min) {
				QMin = xLow + q;
				Min = tmp;
			}
		}
		if (QMin == xLow) {
			points[xLow].push_back(yLow);
		}
		else if ((QMin - xLow - 1) * delta + alpha(X[QMin - 1], Y[yHigh - 1]) <= (QMin - xLow) * delta + delta) {
			for (int x = xLow; x < QMin; ++x) {
				points[x].push_back(yLow);
			}
		}
		else {
			for (int x = xLow; x <= QMin; ++x) {
				points[x].push_back(yLow);
			}
		}
		for (int x = QMin; x <= xHigh; ++x) {
			points[x].push_back(yHigh);
		}
		return Min;
	}
	int yMid = (yLow + yHigh) / 2;
	vector<int> dist1, dist2;
	editDistanceDP(X, Y, xLow, yLow, xHigh, yMid, delta, false, dist1);
	editDistanceDP(X, Y, xLow, yMid, xHigh, yHigh, delta, true, dist2);
	int QMin = xLow;
	int Min = dist1[0] + dist2[xHigh - xLow];
	for (int q = 1; q <= xHigh - xLow; ++q) {
		int tmp = dist1[q] + dist2[xHigh - xLow - q];
		if (tmp < Min) {
			QMin = xLow + q;
			Min = tmp;
		}
	}
	points[QMin].push_back(yMid);
	//cout<< "(" << QMin << ", " << yMid << ") ";
	return editDistanceDC(X, Y, xLow, yLow, QMin, yMid, delta) + editDistanceDC(X, Y, QMin, yMid, xHigh, yHigh, delta);
}

int editDistanceDPDC(string& X, string& Y, const int& delta) {
	return editDistanceDC(X, Y, 0, 0, X.length(), Y.length(), delta);
}

void printAns(string& X, string& Y)
{
	for (int i = 0; i < X.length()+1; ++i) {
		set<int> tmp(points[i].begin(), points[i].end());
		points[i].assign(tmp.begin(), tmp.end());
		sort(points[i].begin(), points[i].end());
	}
	for (int i = 0; i < X.length()+1; ++i) {
		for (int j = 0; j < points[i].size(); ++j) {
			cout << "(" << i << ", " << points[i][j] << ") ";
		}
		cout << endl;
	}
	int pre = -1;
	for (int i = 0; i < X.length()+1; ++i) {
		if (points[i].size() > 1)
		{
			for (int j = 0; j < points[i].size() - 1; ++j) {
				cout << "-";
			}
		}
		cout << X[i];

	}
	cout << endl;
	for (int i = 0; i < X.length()+1; ++i) {
		for (int j = 0; j < points[i].size(); ++j) {
			if (j==0&&pre == points[i][0]) {
				cout << "-";
			}
			else cout << Y[points[i][j]];
		}
		pre = points[i][points[i].size() - 1];
	}
}

void saveans(int isans)
{
	if (isans)
	{
		for (int i = 0; i < anspoints.size(); ++i)
			anspoints[i].clear();
		anspoints = vector<vector<int>>(100);
		anspoints = points;
	}
	for (int i = 0; i < points.size(); ++i)
		points[i].clear();
	points = vector<vector<int>>(100);
}
/*
int main() {
	ifstream Xin("X.txt", ifstream::in);
	ifstream Yin("Y.txt", ifstream::in);
	if (!Xin.is_open() || !Yin.is_open()) {
		cerr << "Error: cannot open input file" << endl;
		exit(-1);
	}
	int delta = mismatch;
	string X, Y;
	Xin >> X;
	Yin >> Y;
	Xin.close();
	Yin.close();
	vector<vector<int>> points(X.length() + 1);
	cout << "DP:\t" << editDistanceDP(X, Y, delta) << endl;
	cout << "DP+DC:\t" << editDistanceDPDC(X, Y, delta, points) << endl;
}
*/