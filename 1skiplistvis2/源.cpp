
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <tchar.h>
#include "../skiplistRedisOO/skiplist.h"
#include<string>
#include<iostream>
using namespace std;

int main () {
#pragma warning(disable:4996)
    //freopen ("../skiplistRedisOO/in.txt", "r", stdin);

    skiplist<long long, string> z;

    //long long n, m;
    //cin >> n >> m;

    //for (long long i = 0; i < n; i++) {
    //    long long  a;
    //    cin >> a;
    //    string b;
    //    cin >> b;
    //    z.insert (a, b);

    //}

    for (long long i = 0; i < 100; i++) {
        //long long a;
        //cin >> a;
        //if (a == 1) {
        //    long long b;
        //    cin >> b;
        //    string* p = z.getDataByScore (b);
        //    if (p) {
        //        cout << *p;
        //    } else {
        //        cout << "-1";
        //    }
        //    cout << "\n";
        //} else if (a == 2) {
        //    long long b;
        //    cin >> b;
        //    string c;
        //    cin >> c;
        //    z.insert (b, c);
        //} else if (a == 3) {
        //    long long b;
        //    cin >> b;
        //    string* p = z.getDataByScore (b);
        //    if (p) {
        //        cout << b << " " << *p;
        //        z.deleteByScore (b);
        //    } else {
        //        cout << "-1";
        //    }
        //    cout << "\n";
        //} else if (a == 4) {
        //    long long* k = z.getMinScore ();
        //    string* e = z.getDataByScore (*k);
        //    if (e) {
        //        cout << *k << " " << *e;
        //    } else {
        //        cout << "-1";
        //    }
        //    z.deleteByScore (*k);

        //    cout << "\n";
        //} else if (a == 5) {
        //    long long* k = z.getMaxScore ();
        //    string* e = z.getDataByScore (*k);

        //    if (e) {
        //        cout << *k << " " << *e;
        //    } else {
        //        cout << "-1";
        //    }
        //    z.deleteByScore (*k);
        //    cout << "\n";
        //}
        z.insert (rand () % 100, "h");
        //Sleep (5);
        //system ("cls");
        //cout << z;
    }
    //初始化


    while (1) {
        system ("cls");
        cout << "                                     ----------------------------------\n";
        cout << "                                     *            跳表控制界面        *\n";
        cout << "                                     *                                *\n";
        cout << "                                     *                                *\n";
        cout << "                                     *                                *\n";
        cout << "                                     *---------------------------------\n";
        cout << z;
        cout << endl;
        cout << "1 key查找\n";
        cout << "2 key value插入\n";
        cout << "3 key删除\n";
        cout << "4 删除最小\n";
        cout << "5 删除最大\n";
        int op, key;
        //cin>>
        long long a;
        cin >> a;
        if (a == 1) {
            long long b;
            cin >> b;
            string* p = z.getDataByScore (b);
            if (p) {
                cout << *p;
            }
            else {
                cout << "-1";
            }
            cout << "\n";
        }
        else if (a == 2) {
            long long b;
            cin >> b;
            string c;
            cin >> c;
            z.insert (b, c);
        }
        else if (a == 3) {
            long long b;
            cin >> b;
            string* p = z.getDataByScore (b);
            if (p) {
                cout << b << " " << *p;
                z.deleteByScore (b);
            }
            else {
                cout << "-1";
            }
            cout << "\n";
        }
        else if (a == 4) {
            long long* k = z.getMinScore ();
            string* e = z.getDataByScore (*k);
            if (e) {
                cout << *k << " " << *e;
            }
            else {
                cout << "-1";
            }
            z.deleteByScore (*k);

            cout << "\n";
        }
        else if (a == 5) {
            long long* k = z.getMaxScore ();
            string* e = z.getDataByScore (*k);

            if (e) {
                cout << *k << " " << *e;
            }
            else {
                cout << "-1";
            }
            z.deleteByScore (*k);
            cout << "\n";
        }
        //cout << z;

    }
}