#include"LoserTree.h"

#include<iostream>

using namespace std;


int main () {
    int* tobesort = new int[100];
    for (int i = 0; i < 10; ++i) {
        tobesort[i] = i * i;
    }
    LoserTree<int> tree;
    tree.initTree (tobesort, 10);


    tree.output (cout);
   
}