#pragma once
#ifndef MSA2_GENETIC_H
#define MSA2_GENETIC_H
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
#include<stdlib.h>
#include<time.h>
#include <algorithm>
using namespace std;
const int Pc = 7, Pv = 10;
struct strpairs {
	string s1;
	string s2;
	int value = 0x0ffffff;
	bool operator < (const strpairs& a)const {
		return value > a.value;
	}
};

vector<int> randomer(int num, int min, int max)
{
	vector<int> returnvec;
	if (num == 0)
	{
		return returnvec;
	}
	srand((unsigned)time(0));
	if (min == max)
	{
		if (num == 1)
		{
			returnvec.push_back(min);
			return returnvec;
		}
		else
		{
			cout << "error";
		}
	}
	if (num == 1) {
		returnvec.push_back(min + (rand() % (max - min+1)));
		return returnvec;
	}
	for (int i = 0; i < num; ++i) {
		returnvec.push_back(min + (rand() % (max - min+1)));
	}
	/*int *a=new int [max - min + 5];
	for (int i = 0; i < max - min + 1; ++i) a[i] = min + i;
	for (int j = max - min; j >= 1; --j) swap(a[j], a[rand() % j]);
	for (int i = 0; i < num; ++i)
		returnvec.push_back(a[i]);
	delete[] a;*/
	return returnvec;
}
vector<int> delrandomer(int num, int min, int max)
{
	vector<int> returnvec;
	if (num == 0)
	{
		return returnvec;
	}
	srand((unsigned)time(0));
	if (min == max)
	{
		if (num == 1)
		{
			returnvec.push_back(min);
			return returnvec;
		}
		else
		{
			cout << "error";
		}
	}
	if (num == 1) {
		returnvec.push_back(min + (rand() % (max - min + 1)));
		return returnvec;
	}
	int *a=new int [max - min + 5];
	for (int i = 0; i < max - min + 1; ++i) a[i] = min + i;
	for (int j = max - min; j >= 1; --j) swap(a[j], a[rand() % j]);
	for (int i = 0; i < num; ++i)
		returnvec.push_back(a[i]);
	delete[] a;
	return returnvec;
}
class population {
public:
	priority_queue<strpairs> member;
	double maxgapratio = 0.3;
	string str1, str2;

	void buildPop(int popsize) {
		if (str1 == str2)return;
		int Maxgap = maxgapratio * min(str1.length(), str2.length());
		for (int w = 0; w < popsize; ++w)
		{
			strpairs* tmp = new strpairs;
			int strlen = rand() % Maxgap + max(str1.length(), str2.length())+1;
			vector<int> randgap1 = randomer(strlen- str1.length(), 0, str1.length() - 1);
			vector<int> randgap2 = randomer(strlen - str2.length(), 0, str2.length() - 1);
			sort(randgap1.begin(), randgap1.end());
			sort(randgap2.begin(), randgap2.end());
			int next = 0;
			for (int i = 0; i < randgap1.size(); ++i)
			{
				for (int j = next; j < randgap1[i]; ++j)
				{
					tmp->s1 = tmp->s1 + str1[j];
				}
				tmp->s1 = tmp->s1 + '-';
				next = randgap1[i];
			}
			if (next != 0)
			{
				tmp->s1 = tmp->s1 + str1.substr(next);
			}
			next = 0;
			for (int i = 0; i < randgap2.size(); ++i)
			{
				for (int j = next; j < randgap2[i]; ++j)
				{
					tmp->s2 = tmp->s2 + str2[j];
				}
				tmp->s2 = tmp->s2 + '-';
				next = randgap2[i];
			}
			if(next!=0)
			tmp->s2 = tmp->s2 + str2.substr(next);
			if (tmp->s2.length() == 0)
			{
				cout << "error1:"<<str2<<"//"<< randgap2.size();
			}
			tmp->value = sum(tmp->s1, tmp->s2);
			member.push(*tmp);
		}
	}

	strpairs varfor1(strpairs a)
	{
		//cout << "invarfor1" << endl;
		int gaplen1 = a.s1.length() - str1.length();	
		int gaplen2 = a.s2.length() - str2.length();
		int mingap = min(gaplen1, gaplen2);
		int maxgap = max(gaplen1, gaplen2);
		string nexts1, nexts2;
		int noise = 0;
		//cout << a.s1.length() << endl;
		for (int i = 0; i < a.s1.length(); ++i)
		{
			if (noise <= 0 && abs(noise) >= mingap)
			{
				nexts1 = nexts1 + a.s1[i];
			}
			else {
				if (a.s1[i] == '-') 
				{
					if((rand() % gaplen1) <  gaplen1*0.6 )
					nexts1 = nexts1 + a.s1[i];
					else --noise;
				}
				else {
					if ((a.s1[i] != '-') && (rand() %(gaplen1+10)) == 0 )
					{
						nexts1 = nexts1 + '-' + a.s1[i];
						++noise;
					}
					else nexts1 = nexts1 + a.s1[i];
				}
				
			}
		}
		//cout << nexts1.length() << endl;
		if (noise == 0)return a;
		//cout << a.s2.length() << endl;
		if (noise < 0)
		{
			vector<int> randgap = delrandomer(abs(noise), 1, gaplen2);
			sort(randgap.begin(), randgap.end());
			int gapnum = 0, pos = 0;
			for (int i = 0; i < a.s2.length(); ++i)
			{
				if (a.s2[i] != '-')
					nexts2 = nexts2 + a.s2[i];
				else {
					++gapnum;
					if (pos<randgap.size()&& gapnum == randgap[pos])
					{
						++pos;
					}else nexts2 = nexts2 + a.s2[i];
				}	
				
			}
		}
		else {
			vector<int> randgap = randomer(abs(noise), 0, a.s2.length());
			sort(randgap.begin(), randgap.end());
			int next = 0;
			for (int i = 0; i < randgap.size(); ++i)
			{
				for (int j = next; j < randgap[i]; ++j)
				{
					nexts2 = nexts2 + a.s2[j];
				}
				nexts2 = nexts2 + '-';
				next = randgap[i];
			}
			nexts2 = nexts2 + a.s2.substr(next);
		}
		//cout << nexts2.length() << endl;
		if (a.s2.length() == 0)cout << "error2";
		a.s1 = nexts1;
		a.s2 = nexts2;
		a.value = sum(nexts1, nexts2);
		//cout << "outvarfor1" << endl;
		return a;


	}

	strpairs variation(strpairs x)
	{
		//cout << "invariation" << endl;
		priority_queue<strpairs> varmember;
		varmember.push(x);
		for (int i = 0; i < Pv; ++i)
		{
			strpairs tmp = varfor1(x);
			varmember.push(tmp);
		}
		//cout << "outvariation" << endl;
		return varmember.top();
	}

	strpairs workfuc(int times)
	{
		int popsize = member.size();
		
		for (int i = 0; i < times; ++i)
		{
			//cout << "infor" << endl;
			strpairs tmp;
			priority_queue<strpairs> nextmember;
			//save the best 15% pairs to the nextpop
			//cout << "next" << endl;
			for (int j = 0; j < ceil(member.size() * 0.15); ++j)
			{
				tmp = member.top();
				member.pop();
				nextmember.push(tmp);
			}
			//cout << "while" << endl;
			while (!member.empty() && nextmember.size() < popsize) {
				tmp = member.top();
				member.pop();
				if (rand() % 10 <= Pc)
				{
					strpairs retmp1 = variation(tmp);
					strpairs retmp2 = variation(tmp);
					nextmember.push(retmp1);
					nextmember.push(retmp2);
				}
			}
			member = nextmember;
			//cout << member.top().value << endl;
			//cout <<"size:"<< member.size() << endl;
			//cout << "outwhile" << endl;
		}
		return member.top();
	}
	
	strpairs Genetic(string X, string Y)
	{
		srand(time(NULL));
		str1 = X;
		str2 = Y;
		strpairs ans;
		int pun = str1.length() - str2.length();
		if (abs(pun)> 20)
		{
			ans.value = 0x0ffffff;
			return ans;
		}
		buildPop(200);
		ans = workfuc(10);
		return ans;
	}
};
#endif


