/*
@Author: TriAlley
@Date: 2020/02/25
@refrence:
	1. Skip Lists : A Probabilistic Alternative to Balanced Trees
	2. https://www.cnblogs.com/gqtcgq/p/7247074.html
*/
#include <iostream>
#include "skipList.h"

using namespace std;
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

    skipList<int, string> z (1000);
    

    int n, m;
    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;
        string b;
        cin >> b;
        TApair<int, string> d (a, b);

        z.insert (d);

    }
    for (int i = 0; i < m; i++) {
        int a;
        cin >> a;
        if (a == 1) {
            int b;
            cin >> b;
            TApair<int ,string>* p= z.find (b);
            if (p) {
                cout << p->second;
            } else {
                cout << "-1";
            }
            cout << "\n";
        } else if (a == 2) {
            int b;
            cin >> b;
            string c;
            cin >> c;
            TApair<int, string> d (b, c);
            z.insert(d);
        } else if (a == 3) {
            int b;
            cin >> b;
            TApair<int, string>* p = z.find (b);
            if (p) {
                z.erase(b);
            } else {
                cout << "-1";
            }
            cout << "\n";
        } else if (a == 4) {

            TApair<int, string>* p = z.getMin ();
            if (p) {
                cout << p->second;
            } else {
                cout << "-1";
            }
            cout << "\n";
        } else if (a == 5) {
            TApair<int, string>* p = z.getMax ();
            if (p) {
                cout << p->second;
            } else {
                cout << "-1";
            }
            cout << "\n";
        }
    }



    return 0;
}
