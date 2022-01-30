// MSA.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstring>
#include <string>
#include<stdio.h>
#include "MSA2_DP.h"
#include "MSA3_DP.h"
#include "MSA2_Astar.h"
#include "MSA2_Genetic.h"
#include "MSA3_Genetic.h"
#include "MSA3_Astar.h"
using namespace std;

struct strpairs;
int alg;

int main()
{

    clock_t start, end;
    FILE* database,* query;
    cout << "Please choose one algorithm (1-DP, 2-Astar, 3-Genetic):";
    cin >> alg;
    if (alg < 1 || alg>3)
        return 0;
 
    char tmp1[200],tmp2[200];
    int q;
    string ans,dist;
    database = fopen("MSA_database.txt", "r");
    vector<string> data;
    string str1, str2, str3;
    while (fgets(tmp2, 200, database) != NULL)
    {
        string str2 = tmp2;
        data.push_back(str2);
    }
    fclose(database);
    strpairs Gans, Gcur;
    query = fopen("MSA_query2.txt", "r");
    if (query == NULL) {
        puts("fail to open files");
        return 0;
    }
    fscanf(query, "%d\n", &q);
    cout << "Query for two-sequence alignment:" << endl;
    start = clock();
    for (int i = 1; i <= 5; ++i)
    {
        fgets(tmp1, 200, query);
        str1 = tmp1;
        cout << "Input: " << str1;
        
        {
            int min = 10000000, minid = -1;
            
            for(int id=0;id<data.size();++id)
            {
                str2 = data[id];
                int cur = min + 1;
                //cout << id << endl;
                if(alg==1)
                    cur = editDistanceDP(str1, str2, 2);
                if (alg == 2)
                    cur = Astar(str1, str2, 0 , min);
                if (alg == 3)
                {
                    population x;
                    Gcur =x.Genetic(str1, str2);
                    cur = Gcur.value;
                }
                
                //int cur = MSAfor2(q, str1, str2);
                if (cur < min)
                {
                    min = cur;
                    minid = id;
                    ans = str2;
                    if (alg == 3)
                    {
                        Gans = Gcur;
                    }
                }
            }

            
            cout << "MinID:" << minid + 1 << endl;
            cout << "MinCost:" << min << endl;
            cout << ans;
            dist = str1;
            if(alg == 1)
                findpath(dist, ans, 2);
            else if (alg == 2)
                Astar(dist, ans, 1, min);
            else if (alg == 3)
            {
                cout << Gans.s1;
                cout << Gans.s2;
            }
        }
        cout << endl;
    }
    end = clock();
    cout << "time:" << (double)(end - start) / CLK_TCK << endl;
    fclose(query);
    //return 0;
    mas3:
    query = fopen("MSA_query3.txt", "r");
    string ans1, ans2;
    fscanf(query, "%d\n", &q);
    cout << "Query for three-sequence alignment:" << endl;
    strpairs3 Gans3, Gcur3;
    start = clock();



    for (int i = 1; i <= 2; ++i)
    {
        fgets(tmp1, 200, query);
        str1 = tmp1;
        if (q == 3)
        {
            int min = 10000000, minid1 = -1, minid2 = -1;

            for (int id1 = 0; id1 < data.size(); ++id1)
            {
                for (int id2 = id1+1; id2 < data.size(); ++id2)
                {
                    str2 = data[id1];
                    str3 = data[id2];
                    int cur = min + 1;
                    if (alg == 1)
                         cur = editDistanceDP3(str1, str2, str3, 2);
                    else if (alg == 2)
                        cur = Astar3(str1, str2, str3, 0, min);
                    else if (alg == 3)
                    {
                        population3 x;
                        Gcur3 = x.Genetic3(str1, str2, str3);
                        cur = Gcur3.value;
                    }
                    if (cur < min)
                    {
                        min = cur;
                        minid1 = id1;
                        minid2 = id2;
                        ans1 = str2;
                        ans2 = str3;
                        if (alg == 3)
                        {
                            Gans3 = Gcur3;
                        }
                    }
                }
            }
            cout << "MinID1:" << minid1 + 1 << endl;
            cout << "MinID2:" << minid2 + 1 << endl;
            cout << "MinCost:" << min << endl;
            if (alg == 1)
                findpath3(str1, ans1, ans2, 2);
            else if(alg==2)
                Astar3(str1, ans1, ans2, 1, min);
            else if (alg == 3)
            {
                cout << Gans3.s1;
                cout << Gans3.s2;
                cout << Gans3.s3;
            }
            //saveans(1);
            //printAns(dist, ans);
        }
        cout << endl;   
    }
    end = clock();
    cout << "time:" << (double)(end - start) / CLK_TCK << endl;
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
