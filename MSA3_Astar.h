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
#include "MSA3_DP.h"
using namespace std;
struct node3 {
public:
	int d = 0;
	int f = 0;
	int x = 0;
	int y = 0;
	int z = 0;
	bool operator < (const node3& a)const {
		return f > a.f;
	}
};
inline int heuristic3(string a, string b,string c)
{
	return editDistanceDP(a,b,2)+ editDistanceDP(a, c,2)+ editDistanceDP(c, b,2);
}

bool goaltest3(node3 a, string& X, string& Y, string& Z)
{
	return (X.length() == a.x) && (Y.length() == a.y) && (Z.length() == a.z);
}

void printAns_Astar3(string X, string Y, string Z, vector<vector<vector<int>>> pre)
{
	int m = X.length();
	int n = Y.length();
	int o = Z.length();
	string ansX = "", ansY = "",ansZ="";
	int i = m, j = n, k = o;
	while (i != 0 || j != 0 || k != 0)
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
	cout << "Alignment:" << endl;
	cout << ansX;
	cout << ansY;
	cout << ansZ;

	cout << sum(ansX, ansY) + sum(ansY, ansZ) + sum(ansX, ansZ) << endl;
}

int Astar3(string& X, string& Y, string& Z, bool isAns, int curans)
{
	int m = X.length();
	int n = Y.length();
	int o = Z.length();
	priority_queue<node3> que;
	vector<vector<vector<int>>> vis(m + 1);
	vector<vector<vector<int>>> pre(m + 1);
	vector<vector<vector<int>>> g(m + 1);
	for (int i = 0; i <= m; ++i) {
		vis[i].resize(n + 1);
		pre[i].resize(n + 1);
		g[i].resize(n + 1);
	}
	for (int i = 0; i <= m; ++i) {
		for (int j = 0; j <= n; ++j)
		{
			vis[i][j] .resize(o + 1);
			pre[i][j].resize(o + 1);
			g[i][j].resize(o + 1);
		}
	}
	node3 tmp;
	tmp.d = 0;
	tmp.f = 0;
	tmp.y = 0;
	tmp.x = 0;
	tmp.z = 0;
	pre[0][0][0] = -2;
	g[m][n][o] = 0x0fffff;
	que.push(tmp);
	while (!que.empty())
	{
		tmp = que.top();
		que.pop();
		if (goaltest3(tmp, X, Y, Z))break;
		if (tmp.f > curans || vis[tmp.x][tmp.y][tmp.z] == 1)continue;
		vis[tmp.x][tmp.y][tmp.z] = 1;
		
		
		//x-1
		if (tmp.x < X.length() && (vis[tmp.x + 1][tmp.y][tmp.z] != 1))
		{
			node3* rightpath = new node3(tmp);
			rightpath->x++;
			rightpath->d += 4;
			rightpath->f = rightpath->d + heuristic3(X.substr(rightpath->x), Y.substr(rightpath->y), Z.substr(rightpath->z));
			if (pre[rightpath->x][rightpath->y][rightpath->z] == 0 || rightpath->d < g[rightpath->x][rightpath->y][rightpath->z])
			{
				pre[rightpath->x][rightpath->y][rightpath->z] = 1;
				g[rightpath->x][rightpath->y][rightpath->z] = rightpath->d;
			}

			que.push(*rightpath);
		}

		//y-2
		if (tmp.y < Y.length() && (vis[tmp.x][tmp.y + 1][tmp.z] != 1))
		{
			node3* downpath = new node3(tmp);
			downpath->y++;
			downpath->d += 4;
			downpath->f = downpath->d + heuristic3(X.substr(downpath->x), Y.substr(downpath->y), Z.substr(downpath->z));
			if (pre[downpath->x][downpath->y][downpath->z] == 0 || downpath->d < g[downpath->x][downpath->y][downpath->z])
			{
				pre[downpath->x][downpath->y][downpath->z] = 2;
				g[downpath->x][downpath->y][downpath->z] = downpath->d;
			}

			que.push(*downpath);
		}

		//z-3
		if (tmp.z < Z.length() && (vis[tmp.x][tmp.y][tmp.z+1] != 1))
		{
			node3* backpath = new node3(tmp);
			backpath->z++;
			backpath->d += 4;
			backpath->f = backpath->d + heuristic3(X.substr(backpath->x), Y.substr(backpath->y), Z.substr(backpath->z));
			if (pre[backpath->x][backpath->y][backpath->z] == 0 || backpath->d < g[backpath->x][backpath->y][backpath->z])
			{
				pre[backpath->x][backpath->y][backpath->z] = 3;
				g[backpath->x][backpath->y][backpath->z] = backpath->d;
			}

			que.push(*backpath);
		}

		//xy-4
		if ((tmp.x < X.length()) && (tmp.y < Y.length()) && (vis[tmp.x + 1][tmp.y + 1][tmp.z] != 1))
		{
			node3* drpath = new node3(tmp);
			drpath->d += alpha(X[drpath->x], Y[drpath->y])+4;
			drpath->x++;
			drpath->y++;
			drpath->f = drpath->d + heuristic3(X.substr(drpath->x), Y.substr(drpath->y), Z.substr(drpath->z));
			if (pre[drpath->x][drpath->y][drpath->z] == 0 || drpath->d < g[drpath->x][drpath->y][drpath->z])
			{
				pre[drpath->x][drpath->y][drpath->z] = 4;
				g[drpath->x][drpath->y][drpath->z] = drpath->d;
			}

			que.push(*drpath);
		}

		//xz-5
		if ((tmp.x < X.length()) && (tmp.z < Z.length()) && (vis[tmp.x + 1][tmp.y ][tmp.z+ 1] != 1))
		{
			node3* rbpath = new node3(tmp);
			rbpath->d += alpha(X[rbpath->x], Z[rbpath->z]) + 4;
			rbpath->x++;
			rbpath->z++;
			rbpath->f = rbpath->d + heuristic3(X.substr(rbpath->x), Y.substr(rbpath->y), Z.substr(rbpath->z));
			if (pre[rbpath->x][rbpath->y][rbpath->z] == 0 || rbpath->d < g[rbpath->x][rbpath->y][rbpath->z])
			{
				pre[rbpath->x][rbpath->y][rbpath->z] = 5;
				g[rbpath->x][rbpath->y][rbpath->z] = rbpath->d;
			}

			que.push(*rbpath);
		}

		//yz-6
		if ((tmp.y < Y.length()) && (tmp.z < Z.length()) && (vis[tmp.x ][tmp.y+ 1][tmp.z + 1] != 1))
		{
			node3* rbpath = new node3(tmp);
			rbpath->d += alpha(Y[rbpath->y], Z[rbpath->z]) + 4;
			rbpath->y++;
			rbpath->z++;
			rbpath->f = rbpath->d + heuristic3(X.substr(rbpath->x), Y.substr(rbpath->y), Z.substr(rbpath->z));
			if (pre[rbpath->x][rbpath->y][rbpath->z] == 0 || rbpath->d < g[rbpath->x][rbpath->y][rbpath->z])
			{
				pre[rbpath->x][rbpath->y][rbpath->z] = 6;
				g[rbpath->x][rbpath->y][rbpath->z] = rbpath->d;
			}

			que.push(*rbpath);
		}

		//xyz-7
		if ((tmp.y < Y.length()) && (tmp.z < Z.length()) && (tmp.x < X.length()) && (vis[tmp.x+1][tmp.y + 1][tmp.z + 1] != 1))
		{
			node3* rbpath = new node3(tmp);
			rbpath->d += alpha(Y[rbpath->y], Z[rbpath->z]) + alpha(Y[rbpath->y], X[rbpath->x])+ alpha(X[rbpath->x], Z[rbpath->z]);
			rbpath->x++;
			rbpath->y++;
			rbpath->z++;
			rbpath->f = rbpath->d + heuristic3(X.substr(rbpath->x), Y.substr(rbpath->y), Z.substr(rbpath->z));
			if (pre[rbpath->x][rbpath->y][rbpath->z] == 0 || rbpath->d < g[rbpath->x][rbpath->y][rbpath->z])
			{
				pre[rbpath->x][rbpath->y][rbpath->z] = 7;
				g[rbpath->x][rbpath->y][rbpath->z] = rbpath->d;
			}

			que.push(*rbpath);
		}
	}
	if (isAns)
		printAns_Astar3(X, Y,Z, pre);
	return g[m][n][o];
}

