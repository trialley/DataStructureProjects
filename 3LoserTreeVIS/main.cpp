#include"LoserTree.h"

#include<iostream>

using namespace std;

/*

3�� ������
����������
Ӧ�þ������ṹģ��ʵ��������
����Ҫ��
��1�����ʵ����С�������ṹADT��ADT��Ӧ������ʼ��������Ӯ�ߣ��ع��Ȼ���������
��2�����ʵ���������ⲿ�����е���������鲢���Լ�K·�鲢��Ӧ����С�������ṹʵ�֣�
��3���������һ���ϳ����ļ������ù鲢·���Լ��������Ĵ�С�����������ķ��ʴ��̵Ĵ��������з������ɲ���С�ļ���ģ����̿顣


*/
int main () {



    int* tobesort = new int[100];
    for (int i = 0; i < 10; ++i) {
        tobesort[i] = i;
    }
    LoserTree<int> tree;
    tree.initTree (tobesort, 10);


    tree.output (cout);
   
}