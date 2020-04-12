#pragma once
// winner tree as a complete binary tree mapped into an array
// derives from the ADT winnerTree

#ifndef completeWinnerTree_
#define completeWinnerTree_

#include <iostream>
#include "winnerTree.h"

template<class T>
class completeWinnerTree : public winnerTree<T> {
public:
    completeWinnerTree () {
        tree = NULL;
    }
    completeWinnerTree (T* thePlayer, int playersAmount, int flag) {
        tree = NULL;
        initialize (thePlayer, playersAmount, flag);
    }
    ~completeWinnerTree () { delete[] tree; }
    void initialize (T*, int, int);
    int winner () const {
        return tree[1];
    }
    int winner (int i) const {
        return (i < numberOfPlayers) ? tree[i] : 0;
    }
    // return winner of match at node i
    void smallWinRePlay (int thePlayer);
    void smallGenerateWinRePlay (int thePlayer);
    void bigWinRePlay (int thePlayer);
    void bigGenerateWinRePlay (int thePlayer);
    void output () const;
    void Kway_mergerwithWT (T* array, int arrayLength, int p, int flag);
private:
    int lowExt;           // ��ײ��ⲿ�ڵ����
    int offset;           // 2*��2^log(n-1)�� - 1����ײ��ڲ��ڵ������е����һ��������һ���������ڼ���ÿ���ⲿ�ڵ�ĸ��ڲ��ڵ�ʱʹ�á�
    int* tree;            // tree[i]����Ӯ�߱��id
    int numberOfPlayers;
    T* player;            // player���飬Ϊ�˴�ÿ��player��ֵ
    int* merge_num;
    void smallWinplay (int, int, int);
    //      void smallGenerateWinplay(int, int, int);
    void bigWinplay (int, int, int);
    //      void bigGenerateWinplay(int, int, int);
};


#endif

