#pragma once
// Ӯ����������

#ifndef winnerTree_
#define winnerTree_

using namespace std;

template<class T>
class winnerTree {
public:
    virtual ~winnerTree () {}
    virtual void initialize (T* thePlayer, int theNumberOfPlayers, int flag) = 0;
    // ����һ����theNumberOfPlayers�������ߵ�Ӯ����
    virtual int winner () const = 0;
    // ��������Ӯ�ߵ��±�
    virtual void bigWinRePlay (int thePlayer) = 0;
    virtual void smallWinRePlay (int thePlayer) = 0;
    // ��ѡ��thePLayer��ֵ�ı�ʱ�����ⲿ�ڵ�player��thePlayer������tree[1]��·���ϣ�һ���ֻ�ȫ����������Ҫ������
};
#endif


