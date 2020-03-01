

#include<string>
#include"skiplist.h"
#include<iostream>
using namespace std;

/*
apple
-1
5 oops
1 mi
10 apple
*/

/*输入包含 1+n+m 行数据
第一行包含两个数字 n, m (0 \leq n \leq 1000, 0 \leq m \leq 10000≤n≤1000,0≤m≤1000)
之后的 n 行，每行包含一个数字与一个字符串，表示一个元素，数字为跳表的关键字(0\leq key \leq 1,000,000,000)(0≤key≤1,000,000,000)，字符串为元素的值。
之后的 m 行表示 m 个跳表ADT操作。每行第一个数字 a 用来区别操作。

a 为 1 时，表示查找操作，其后的一个数字 b 为要查询的元素的关键字， 输出 该元素的值，不存在则输出 -1。
a 为 2 时，表示插入操作，其后的一个数字 b 为要插入的元素，一个字符串 c 为对应的值， 不输出 。
a 为 3 时，表示删除操作，其后的一个数字 b 为要删除的关键字， 输出 该关键字与其对应的元素值，若删除关键字不存在或删除失败，输出 -1。
a 为 4 时，表示删除最小元素操作。 输出 跳表内最小关键字 c 与其对应的元素值，并将对应元素在跳表内部删除
a 为 5 时，表示删除最大元素操作。 输出 跳表内最大关键字 c 与其对应的元素值，并将对应元素在跳表内部删除*/
int main () {
#pragma warning(disable:4996)
    freopen ("in.txt", "r", stdin);

    skiplist<long long, string> z;

    long long n, m;
    cin >> n >> m;

    for (long long i = 0; i < n; i++) {
        long long  a;
        cin >> a;
        string b;
        cin >> b;
        z.insert (a, b);

    }


    for (long long i = 0; i < m; i++) {
        long long a;
        cin >> a;
        if (a == 1) {
            long long b;
            cin >> b;
            string* p = z.skiplistGetDataByScore (b);
            if (p) {
                cout << *p;
            } else {
                cout << "-1";
            }
            cout << "\n";
        } else if (a == 2) {
            long long b;
            cin >> b;
            string c;
            cin >> c;
            z.insert (b, c);
        } else if (a == 3) {
            long long b;
            cin >> b;
            string* p = z.skiplistGetDataByScore (b);
            if (p) {
                cout << b << " " << *p;
                z.skiplistDeleteByScore (b);
            } else {
                cout << "-1";
            }
            cout << "\n";
        } else if (a == 4) {
            string* e = z.getMin ();
            long long* k = z.getMinScore ();
            if (e) {
                cout << *k << " " << *e;
            } else {
                cout << "-1";
            }
            z.skiplistDeleteByScore (*k);

            cout << "\n";
        } else if (a == 5) {
            string* e = z.getMax ();
            long long* k = z.getMaxScore ();

            if (e) {
                cout << *k << " " << *e;
            } else {
                cout << "-1";
            }
            z.skiplistDeleteByScore (*k);
            cout << "\n";
        }
    }


}