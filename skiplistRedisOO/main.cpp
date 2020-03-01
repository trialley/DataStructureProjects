

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

/*������� 1+n+m ������
��һ�а����������� n, m (0 \leq n \leq 1000, 0 \leq m \leq 10000��n��1000,0��m��1000)
֮��� n �У�ÿ�а���һ��������һ���ַ�������ʾһ��Ԫ�أ�����Ϊ����Ĺؼ���(0\leq key \leq 1,000,000,000)(0��key��1,000,000,000)���ַ���ΪԪ�ص�ֵ��
֮��� m �б�ʾ m ������ADT������ÿ�е�һ������ a �������������

a Ϊ 1 ʱ����ʾ���Ҳ���������һ������ b ΪҪ��ѯ��Ԫ�صĹؼ��֣� ��� ��Ԫ�ص�ֵ������������� -1��
a Ϊ 2 ʱ����ʾ�������������һ������ b ΪҪ�����Ԫ�أ�һ���ַ��� c Ϊ��Ӧ��ֵ�� ����� ��
a Ϊ 3 ʱ����ʾɾ������������һ������ b ΪҪɾ���Ĺؼ��֣� ��� �ùؼ��������Ӧ��Ԫ��ֵ����ɾ���ؼ��ֲ����ڻ�ɾ��ʧ�ܣ���� -1��
a Ϊ 4 ʱ����ʾɾ����СԪ�ز����� ��� ��������С�ؼ��� c �����Ӧ��Ԫ��ֵ��������ӦԪ���������ڲ�ɾ��
a Ϊ 5 ʱ����ʾɾ�����Ԫ�ز����� ��� ���������ؼ��� c �����Ӧ��Ԫ��ֵ��������ӦԪ���������ڲ�ɾ��*/
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