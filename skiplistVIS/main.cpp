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
    Stopwatch () : start (clock ()) {} //���캯������������㿪ʼ��ʱ
    ~Stopwatch ();
private:
    clock_t start;
};
Stopwatch::~Stopwatch () {
    clock_t total = clock () - start; //���������͹��캯����ʱ��Ϊ����ʱ��
    cout << "�˲�������ʱ��:" << total << "ms\n";
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
    cout << "mapɾ��ͳ��:\n";
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 500; i++) {
            n.erase(rand () % 100000);
        }
    }
    cout << "map����ͳ��:\n";
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
    cout << "skiplistɾ��ͳ��:\n";
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 500; i++) {
            x.deleteByScore (rand () % 100000);
        }
    }
    cout << "skiplist����ͳ��:\n";
    {
        srand (1);
        Stopwatch watch;
        for (int i = 0; i < 500; i++) {
            x.insert (rand () % 100000,i);
        }
    }
    cout << "\nvector�ؼ��ʲ�����:\n";

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