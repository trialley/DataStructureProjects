#pragma once
// 赢者树纯虚类

#ifndef winnerTree_
#define winnerTree_

using namespace std;

template<class T>
class winnerTree {
public:
    virtual ~winnerTree () {}
    virtual void initialize (T* thePlayer, int theNumberOfPlayers, int flag) = 0;
    // 创建一个有theNumberOfPlayers个竞赛者的赢者树
    virtual int winner () const = 0;
    // 返回最终赢者的下标
    virtual void bigWinRePlay (int thePlayer) = 0;
    virtual void smallWinRePlay (int thePlayer) = 0;
    // 当选手thePLayer的值改变时，从外部节点player【thePlayer】到根tree[1]的路径上，一部分或全部比赛都需要重赛。
};
#endif


