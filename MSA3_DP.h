#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;
//vector<vector<int>> points(200), anspoints(200);
//const int Mismatch = 3, gap = 2;
inline int alpha2(char c1, char c2) {
	if (c1 == c2) return 4;
	return 3+4;
}
int sum(string A, string B)
{
	int sum = 0;
	if (A.length() != B.length())
	{
		return 0x0ffffff;
	}
		
	for (int i = 0; i < A.length(); ++i)
	{
		if (A[i] == B[i])continue;
		if (A[i] == '-' || B[i] == '-')
			sum += 2;
		else sum += 3;
	}
	return sum;
}
inline int alpha3(char c1, char c2, char c3) {
	int mismatchs = (c1 != c2) + (c2 != c3) + (c1 != c3);
	return 3 * mismatchs;
}
inline int zerofilter(int input) {
	if (input >= 0)return input;
	return 0;
}

int editDistanceDP3(string& X, string& Y, string& Z, const int& delta) {
	int m = X.length();
	int n = Y.length();
	int o = Z.length();
	vector<vector<vector<int>>> dp(m + 1);
	for (int i = 0; i <= m; ++i) {
		dp[i].resize(n + 1);
	}
	for (int i = 0; i <= m; ++i) {
		for(int j = 0; j <= n; ++j)
			dp[i][j].resize(o + 1);
	}
	dp[0][0][0] = 0;
	for (int i = 0; i <= m; ++i) {
		if (i > 0)
			dp[i][0][0] = dp[i - 1][0][0] + 2*delta;
		for (int j = 0; j <= n; ++j) {
			if (j > 0)
			{
				dp[i][j][0] = dp[i][j - 1][0] + 2*delta;
				if (i > 0) {
					dp[i][j][0] = min(dp[i][j][0], dp[i - 1][j][0] + 2*delta);
					dp[i][j][0] = min(dp[i][j][0], dp[i - 1][j - 1][0] + alpha2(X[i - 1], Y[j - 1]));
				}
			}
			for (int k = 1; k <= o; ++k) {
				dp[i][j][k] = dp[i][j][k - 1] + 2*delta;
				if (i > 0)
				{
					dp[i][j][k] = min(dp[i][j][k], dp[i - 1][j][k] + 2*delta);
					dp[i][j][k] = min(dp[i][j][k], dp[i - 1][j][k-1] + alpha2(X[i - 1], Z[k - 1]));
				}
				if (j > 0)
				{
					dp[i][j][k] = min(dp[i][j][k], dp[i][j - 1][k] + 2*delta);
					dp[i][j][k] = min(dp[i][j][k], dp[i][j - 1][k - 1] + alpha2(Y[j - 1], Z[k - 1]));
				}
				if (i > 0 && j > 0)
				{
					dp[i][j][k] = min(dp[i][j][k], dp[i - 1][j - 1][k - 1] + alpha3(X[i - 1], Y[j - 1], Z[k - 1]));
					dp[i][j][k] = min(dp[i][j][k], dp[i - 1][j - 1][k] + alpha2(X[i - 1], Y[j - 1]));
				}
					
			}
				
			
		}
	}
	return dp[m][n][o];
}

void findpath3(string& X, string& Y, string& Z, const int& delta)
{
	int m = X.length();
	int n = Y.length();
	int o = Z.length();
	vector<vector<vector<int>>> dp(m + 1);
	vector<vector<vector<int>>> pre(m + 1);
	for (int i = 0; i <= m; ++i) {
		dp[i].resize(n + 1);
		pre[i].resize(n + 1);
	}
	for (int i = 0; i <= m; ++i) {
		for (int j = 0; j <= n; ++j)
		{
			dp[i][j].resize(o + 1);
			pre[i][j].resize(o + 1);
		}	
	}
	dp[0][0][0] = 0;
	pre[0][0][0] = 0;
	for (int i = 0; i <= m; ++i) {
		if (i > 0)
		{
			dp[i][0][0] = dp[i - 1][0][0] + 2 * delta;
			pre[i][0][0] = 1;
		}
		for (int j = 0; j <= n; ++j) {
			if (j > 0)
			{
				dp[i][j][0] = dp[i][j - 1][0] + 2 * delta;
				pre[i][j][0] = 2;
				if (i > 0) {
					dp[i][j][0] = min(dp[i][j][0], dp[i - 1][j][0] + 2 * delta);
					dp[i][j][0] = min(dp[i][j][0], dp[i - 1][j - 1][0] + alpha2(X[i - 1], Y[j - 1]));
					if (dp[i][j][0] == dp[i - 1][j][0] + 2 * delta)
						pre[i][j][0] = 1;
					else if (dp[i][j][0] == dp[i - 1][j - 1][0] + alpha2(X[i - 1], Y[j - 1]))
						pre[i][j][0] = 4;
				}
			}
			for (int k = 1; k <= o; ++k) {
				dp[i][j][k] = dp[i][j][k - 1] + 2 * delta;
				pre[i][j][k] = 3;
				if (i > 0)
				{
					dp[i][j][k] = min(dp[i][j][k], dp[i - 1][j][k] + 2 * delta);
					dp[i][j][k] = min(dp[i][j][k], dp[i - 1][j][k - 1] + alpha2(X[i - 1], Z[k - 1]));
					if (dp[i][j][k] == dp[i - 1][j][k] + 2 * delta)
						pre[i][j][k] = 1;
					else if (dp[i][j][k] == dp[i - 1][j][k - 1] + alpha2(X[i - 1], Z[k - 1]))
						pre[i][j][k] = 5;
				}
				if (j > 0)
				{
					dp[i][j][k] = min(dp[i][j][k], dp[i][j - 1][k] + 2 * delta);
					dp[i][j][k] = min(dp[i][j][k], dp[i][j - 1][k - 1] + alpha2(Y[j - 1], Z[k - 1]));
					if (dp[i][j][k] == dp[i][j - 1][k] + 2 * delta)
						pre[i][j][k] = 2;
					else if (dp[i][j][k] == dp[i][j - 1][k - 1] + alpha2(Y[j - 1], Z[k - 1]))
						pre[i][j][k] = 6;
				}
				if (i > 0 && j > 0)
				{
					dp[i][j][k] = min(dp[i][j][k], dp[i - 1][j - 1][k - 1] + alpha3(X[i - 1], Y[j - 1], Z[k - 1]));
					dp[i][j][k] = min(dp[i][j][k], dp[i - 1][j - 1][k] + alpha2(X[i - 1], Y[j - 1]));
					if (dp[i][j][k] == dp[i - 1][j - 1][k - 1] + alpha3(X[i - 1], Y[j - 1], Z[k - 1]))
						pre[i][j][k] = 7;
					else if (dp[i][j][k] == dp[i - 1][j - 1][k] + alpha2(X[i - 1], Y[j - 1]))
						pre[i][j][k] = 4;
				}

			}


		}
	}
	//return dp[m][n][o];
	string ansX="", ansY="", ansZ= "";
	int i = m, j = n, k = o;
	while (i != 0 || j != 0||k!=0)
	{
		switch (pre[i][j][k]) {
		case 1:
			ansX = X[--i] + ansX;
			ansY = '-' + ansY;
			ansZ = '-' + ansZ;
			break;
		case 2:
			ansX = '-' + ansX;
			ansY = Y[--j] + ansY;
			ansZ = '-' + ansZ;
			break;
		case 3:
			ansX = '-' + ansX;
			ansY = '-' + ansY;
			ansZ = Z[--k] + ansZ;
			break;
		case 4:
			ansX = X[--i] + ansX;
			ansY = Y[--j] + ansY;
			ansZ = '-' + ansZ;
			break;
		case 5:
			ansX = X[--i] + ansX;
			ansY = '-' + ansY;
			ansZ = Z[--k] + ansZ;
			break;
		case 6:
			ansX = '-' + ansX;
			ansY = Y[--j] + ansY;
			ansZ = Z[--k] + ansZ;
			break;
		case 7:
			ansX = X[--i] + ansX;
			ansY = Y[--j] + ansY;
			ansZ = Z[--k] + ansZ;
			break;
		}
	}
	cout << ansX;
	cout << ansY;
	cout << ansZ;
	
	cout << sum(ansX, ansY) + sum(ansY , ansZ) + sum(ansX , ansZ)<<endl;
}
