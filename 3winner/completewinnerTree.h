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
    int lowExt;           // 最底层外部节点个数
    int offset;           // 2*（2^log(n-1)） - 1，最底层内部节点所在行的最后一个参数，一个参量，在计算每个外部节点的父内部节点时使用。
    int* tree;            // tree[i]代表赢者编号id
    int numberOfPlayers;
    T* player;            // player数组，为了存每个player的值
    int* merge_num;
    void smallWinplay (int, int, int);
    //      void smallGenerateWinplay(int, int, int);
    void bigWinplay (int, int, int);
    //      void bigGenerateWinplay(int, int, int);
};


#endif

