#pragma once

#include<iostream>
#include<vector>
#include<map>
#pragma warning(disable:4996)

#include"Tree.h"
using namespace std;
template<class T>
class LoserTree {
public:
    LoserTree () { _tree_root = nullptr; };
    LoserTree (T* thePlayers, int n) { initTree (thePlayers, n); }            //����������
    ~LoserTree () { delete[] _tree_root; delete[] temp; } //�ͷ���Դ

    //��������ʼ��
    void initTree (T* thePlayers, int n) {
        
        //����player[l], _players[r]��ֵ������tree[p]��ֵ
        auto _play = [this](int p, int left, int right) ->void {
            //���������ӽڵ�l��r֮��ٰ����
            //֮�����p���Һ��ӽڵ㣬�����������һ��ı���

            _tree_root[p] = _getLose (left, right);
            temp[p] = _getWin (left, right);
            //�ڽڵ�p���Һ����ҷǸ�
            while (p % 2 == 1 && p > 1) {
                _tree_root[p / 2] = _getLose (temp[p - 1], temp[p]);
                temp[p / 2] = _getWin (temp[p - 1], temp[p]);

                p /= 2;
            }
        };
        _tree_root = nullptr, temp = nullptr;

        if (n < 2) { return; }
        _players = thePlayers;
        _player_num = n;

        _tree_root = new int[n];
        temp = new int[n];

        int i, s;

        //����s, lowExt, offset
        for (s = 1; 2 * s <= n - 1; s *= 2);
        lowExt = 2 * (n - s);
        offset = 2 * s - 1;

        //�����еײ��ⲿ�ڵ����play()
        for (i = 2; i <= lowExt; i += 2)
            _play ((i + offset) / 2, i - 1, i);

        //nΪ����ʱ�������һ���ڽڵ��亢�ӷֱ����ڽڵ����ڵ�
        //�ȶ�����ڽڵ����ڵ����play() 
        if (n % 2 == 1) {
            _play (n / 2, temp[n - 1], lowExt + 1);
            i = lowExt + 3;
        } else {
            i = lowExt + 2;
        }

        //�ٶ�ʣ����ⲿ�ڵ�play()
        for (; i <= n; i += 2)
            _play ((i - lowExt + n - 1) / 2, i - 1, i);

        //�����յ�Ӯ�߼�¼��tree[0]
        _tree_root[0] = temp[1];
    }
    
    //���� ע�⣬�л�Ԫ�����ⲿ���
    void reBuild (int thePlayer) {
        int n = _player_num;
        if (thePlayer <= 0 || thePlayer > n) {
            cout << "_players index is illgal" << endl;
            return;
        }

        //get the left _players and the right _players
        int matchPoint;     //����ѡ�ֵĸ��׽ڵ�
        int left, right;     //����ѡ��

        if (thePlayer <= lowExt) {       //the _players is on the bottom 
            matchPoint = (thePlayer + offset) / 2;
            left = 2 * matchPoint - offset;    // unify to the left _players
            right = left + 1;
        } else {   //the _players is on the last but one
            matchPoint = (thePlayer - lowExt + n - 1) / 2;
            //theplayer�����ֵ������һ���ڲ��ڵ�
            if (2 * matchPoint == n - 1) {
                left = temp[2 * matchPoint];
                right = thePlayer;
            } else {
                left = 2 * matchPoint - (n - 1) + lowExt;
                right = left + 1;
            }
        }

        //���±���
        //���±�����ѡ����֮ǰʤ�ߵ�λ��
        if (thePlayer == _tree_root[0]) {//the _players to reBuild is the previous winner!!!,ok,that's simple
            for (; matchPoint >= 1; matchPoint /= 2) {   //�ϴα����������Ѿ���¼��tree[]�� 
                int loserTemp = _getLose (_tree_root[matchPoint], thePlayer);
                temp[matchPoint] = _getWin (_tree_root[matchPoint], thePlayer);
                _tree_root[matchPoint] = loserTemp;
                thePlayer = temp[matchPoint];
            }
        } else {
            //����Ӹýڵ㵽����·�������±���
            _tree_root[matchPoint] = _getLose (left, right);  //first game
            temp[matchPoint] = _getWin (left, right);
            if (matchPoint == n - 1 && n % 2 == 1) {   //second game,and it's special
                matchPoint /= 2;
                _tree_root[matchPoint] = _getLose (temp[n - 1], lowExt + 1);  //fisrt game
                temp[matchPoint] = _getWin (temp[n - 1], lowExt + 1);
            }
            matchPoint /= 2;  //then all is back to normal,continue competing
            for (; matchPoint >= 1; matchPoint /= 2) {
                _tree_root[matchPoint] = _getLose (temp[2 * matchPoint], temp[2 * matchPoint + 1]);
                temp[matchPoint] = _getWin (temp[2 * matchPoint], temp[2 * matchPoint + 1]);
            }
        }
        _tree_root[0] = temp[1];  //place the new winner
    }
    void output (ostream& out) const {
        out << "��Ա���ݣ�" << endl;
        for (int i = 0; i < _player_num; i++) {
            out << _players[_tree_root[i]-1] << " ";
        }
        out << endl;
        out << endl;

        out << "�������ṹ��" << endl;
        //������
        TreeNode<int>* nodes = new TreeNode<int>[100];
        for (int i = 0; i < _player_num; ++i) {
            nodes[i]._index = _tree_root[i];
            nodes[i]._data = _players[_tree_root[i]-1];
        }
        Tree<int> temptree(100,_tree_root[0], _players[_tree_root[0]-1]);
        for (int i = 0; i < _player_num; ++i) {
            if (i * 2 + 1 < _player_num)temptree.addLeftSubTreeByIndex (new TreeNode<int>(nodes[i * 2 + 1]), nodes[i]._index);
            if (i * 2 + 2 < _player_num)temptree.addRightSubTreeByIndex (new TreeNode<int> (nodes[i * 2 + 2]), nodes[i]._index);
        }

        temptree.show (out);



        //delete temptree;/
        delete[]nodes;

    }
    int winner () { return temp[1]; }//��ȡʤ��

    //int* theTree () { return _tree_root; }
private:
    int* _tree_root;       //�ڲ��ڵ㣬���������ߣ�tree[0]��ŵ������ձ�����Ӯ��
    int* temp;       //��ű�����Ӯ��
    T* _players;     //��Ҫ������ѡ��(�ⲿ�ڵ�)

    int _player_num; //ѡ�ֵĸ���

    int lowExt;      //2*(n-s),��ײ��ⲿ�ڵ�ĸ���,s=2^(log2(n-1))
                     //sΪ��ײ�����˵��ڲ��ڵ��λ��
    int offset;      //2*s-1

    int _getWin (int x, int y) { return _players[x] <= _players[y] ? x : y; }    //����Ӯ��
    int _getLose (int x, int y) { return _players[x] <= _players[y] ? y : x; }      //��������
    
    //char charmap[1000][1000];
    //void printBTree () {
    //    for (int i = 0; i < 1000; i++) {
    //        for (int j = 0; j < 1000; j++) {
    //            charmap[i][j] = ' ';
    //        }
    //        charmap[i][1000 - 1] = '\0';

    //    }
    //    int row = 0;
    //    int deepth = 0;
    //    printBTreein (_root, row, deepth);

    //    for (int i = 0; i < 50; i++) {
    //        cout << charmap[i] << endl;
    //    }
    //}
    //int printBTreein (TreeNode<T>* rootin, int& row, int deepthin) {
    //    if (rootin == nullptr) {
    //        return row++;
    //    }
    //    int top = printBTreein (rootin->_leftChild, row, deepthin + 1);

    //    int me = row++;
    //    if (top != me - 1) {
    //        charmap[top][deepthin * 3] = '/';
    //        charmap[top][deepthin * 3 + 1] = '-';
    //        charmap[top][deepthin * 3 + 2] = '-';
    //    }
    //    int bot = printBTreein (rootin->_rightChild, row, deepthin + 1);
    //    if (bot != me + 1) {
    //        charmap[bot][deepthin * 3] = '\\';
    //        charmap[bot][deepthin * 3 + 1] = '-';
    //        charmap[bot][deepthin * 3 + 2] = '-';
    //    }





    //    for (int i = top + 1; i < bot; i++) {
    //        charmap[i][deepthin * 3] = '|';
    //    }

    //    charmap[me][deepthin * 3] = '+';
    //    sprintf (&charmap[me][deepthin * 3], "%d", rootin->_index);
    //    return me;
    //}

};

