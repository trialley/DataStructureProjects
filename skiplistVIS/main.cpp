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


int main () {
#pragma warning(disable:4996)

    map<long long, string> m;
    {
        Stopwatch watch;
        for (long long i = 0; i < 100000; i++) {
            m[rand () % 100000] = "h";
        }
    }
    map<long long, string> n;
    {
        Stopwatch watch;
        for (long long i = 0; i < 100000; i++) {
            n[rand () % 100000] = "h";
        }
    }
    skiplist<long long, string> z;
    {
        Stopwatch watch;
        for (long long i = 0; i < 100000; i++) {
            z.insert (rand () % 100000, "h");
        }
    }
    skiplist<long long, string> x;
    {
        Stopwatch watch;
        for (long long i = 0; i < 100000; i++) {
            x.insert (rand () % 100000, "h");
        }
    }




    //system ("cls");
    //cout << z;
}