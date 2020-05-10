#include"LoserTree.h"

#include<iostream>

using namespace std;

/*

3、 外排序
问题描述：
应用竞赛树结构模拟实现外排序。
基本要求：
（1）设计实现最小输者树结构ADT，ADT中应包括初始化、返回赢者，重构等基本操作。
（2）设计实现外排序，外部排序中的生成最初归并串以及K路归并都应用最小输者树结构实现；
（3）随机创建一个较长的文件；设置归并路数以及缓冲区的大小；获得外排序的访问磁盘的次数并进行分析。可采用小文件来模拟磁盘块。


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