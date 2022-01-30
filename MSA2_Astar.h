#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include "MSA2_DP.h"

using namespace std;
struct node {
public:
	int d = 0;
	int f = 0;
	int x = 0;
	int y = 0;
	bool operator < (const node& a)const {
		return f > a.f;
	}
};
inline int heuristic2(string a, string b)
{
	int tmp = a.length() - b.length();
	return 2 * abs(tmp);
}

bool goaltest(node a, string& X, string& Y)
{
	return (X.length() == a.x) && (Y.length() == a.y);
}

void printAns_Astar(string X, string Y, vector<vector<int>> pre)
{
	int m = X.length();
	int n = Y.length();
	string ansX="", ansY="";
	int i = m, j = n;
	while (i != 0 || j != 0)
	{
		if (pre[i][j] == 2)
		{
			ansX = X[i - 1] + ansX;
			ansY = '-' + ansY;
			--i;
		}
		else if (pre[i][j] == 3)
		{
			ansX = X[i - 1] + ansX;
			ansY = Y[j - 1] + ansY;
			--i;
			--j;
		}
		else if (pre[i][j] == 1) {
			ansX = '-' + ansX;
			ansY = Y[j - 1] + ansY;
			--j;
		}
		else break;
	}
	cout << "Alignment:" << endl;
	cout << ansX;
	cout << ansY;
	cout << sum(ansX, ansY);
}

int Astar(string& X, string& Y,bool isAns, int curans)
{
	int m = X.length();
	int n = Y.length();
	priority_queue<node> que;
	vector<vector<int>> vis(m + 1);
	vector<vector<int>> pre(m + 1);
	vector<vector<int>> g(m + 1);
	for (int i = 0; i <= m; ++i) {
		vis[i].resize(n + 1);
		pre[i].resize(n + 1);
		g[i].resize(n + 1);
	}
	node tmp;
	tmp.d = 0;
	tmp.f = 0;
	tmp.y = 0;
	tmp.x = 0;
	pre[0][0] = -2;
	que.push(tmp);
	g[m][n] = 0x0fffff;
	while (!que.empty())
	{
		tmp = que.top();
		que.pop();
		if (tmp.f > curans || vis[tmp.x][tmp.y] == 1)continue;
		vis[tmp.x][tmp.y] = 1;
		//if (goaltest(tmp, X, Y))break;
		if (tmp.y < Y.length()&& (vis[tmp.x][tmp.y+1] != 1))
		{
			node* downpath = new node(tmp);
			downpath->y++;
			downpath->d += 2;
			downpath->f = downpath->d + heuristic2(X.substr(downpath->x),Y.substr(downpath->y));
			if (pre[downpath->x][downpath->y] == 0 || downpath->d < g[downpath->x][downpath->y])
			{
				pre[downpath->x][downpath->y] = 1;
				g[downpath->x][downpath->y] = downpath->d;
			}
				
			que.push(*downpath);
		}
		
		if (tmp.x < X.length() && (vis[tmp.x+1][tmp.y] != 1))
		{
			node* rightpath = new node(tmp);
			rightpath->x++;
			rightpath->d += 2;
			rightpath->f = rightpath->d + heuristic2(X.substr(rightpath->x), Y.substr(rightpath->y));
			if (pre[rightpath->x][rightpath->y] == 0 || rightpath->d < g[rightpath->x][rightpath->y])
			{
				pre[rightpath->x][rightpath->y] = 2;
				g[rightpath->x][rightpath->y] = rightpath->d;
			}
				
			que.push(*rightpath);
		}
		
		if ((tmp.x < X.length()) && (tmp.y < Y.length())&& (vis[tmp.x + 1][tmp.y+1] != 1))
		{
			node* drpath = new node(tmp);
			drpath->d += alpha(X[drpath->x], Y[drpath->y]);
			drpath->x++;
			drpath->y++;
			drpath->f = drpath->d + heuristic2(X.substr(drpath->x), Y.substr(drpath->y));
			if (pre[drpath->x][drpath->y] == 0 || drpath->d < g[drpath->x][drpath->y])
			{
				pre[drpath->x][drpath->y] = 3;
				g[drpath->x][drpath->y] = drpath->d;
			}
				
			que.push(*drpath);
		}
	}
	if(isAns)
		printAns_Astar(X, Y, pre);
	return g[m][n];
}

