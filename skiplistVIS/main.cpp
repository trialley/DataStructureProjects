#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <tchar.h>
#include "../skiplistRedisOO/skiplist.h"
#include<string>
#include<map>
#include<iostream>
using namespace std;
#include <time.h>
#include <iostream>
#include<vector>
using namespace std;

class Stopwatch {
public:
    Stopwatch () : start (clock ()) {} //构造函数，创建对象便开始计时
    ~Stopwatch ();
private:
    clock_t start;
};
Stopwatch::~Stopwatch () {
    clock_t total = clock () - start; //析构函数和构造函数的时间差即为所用时间
    cout << "此操作所用时间:" << total << "ms\n";
}
struct t {
    int f;
    int d;
};

int main () {
#pragma warning(disable:4996)
    cout << "\nmap:\n";
    map<int, int> m;
    {
        Stopwatch watch;
        srand (1);
        for (int i = 0; i < 100000; i++) {
            m[rand () % 100000] = i;
        }
    }
    map<int, int> n;
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 100000; i++) {
            n[rand () % 100000] = i;
        }
    }
    cout << "map删除统计:\n";
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 500; i++) {
            n.erase(rand () % 100000);
        }
    }
    cout << "map插入统计:\n";
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 500; i++) {
            n [rand () % 100000]=i;
        }
    }
    cout << "\nskiplist:\n";

    skiplist<int, int> z;
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 100000; i++) {
            z.insert (rand () % 100000, i);
        }
    }
    skiplist<int, int> x;
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 100000; i++) {
            x.insert (rand () % 100000, i);
        }
    }
    cout << "skiplist删除统计:\n";
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 500; i++) {
            x.deleteByScore (rand () % 100000);
        }
    }
    cout << "skiplist插入统计:\n";
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 500; i++) {
            x.insert (rand () % 100000,i);
        }
    }
    cout << "\nvector关键词不排序:\n";

    vector<t> v;
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 100000; i++) {
            v.push_back ({ rand () % 100000, i });
        }
    }
    vector<t> v1;
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 100000; i++) {
            v1.push_back ({ rand () % 100000, i });
        }
    }
}