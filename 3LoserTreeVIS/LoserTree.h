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
    LoserTree (T* thePlayers, int n) { initTree (thePlayers, n); }            //创建输者树
    ~LoserTree () { delete[] _tree_root; delete[] temp; } //释放资源

    //败者树初始化
    void initTree (T* thePlayers, int n) {
        
        //根据player[l], _players[r]的值，设置tree[p]的值
        auto _play = [this](int p, int left, int right) ->void {
            //即在左右子节点l，r之间举办比赛
            //之后如果p是右孩子节点，则继续举行上一层的比赛

            _tree_root[p] = _getLose (left, right);
            temp[p] = _getWin (left, right);
            //内节点p是右孩子且非根
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

        //计算s, lowExt, offset
        for (s = 1; 2 * s <= n - 1; s *= 2);
        lowExt = 2 * (n - s);
        offset = 2 * s - 1;

        //对所有底层外部节点进行play()
        for (i = 2; i <= lowExt; i += 2)
            _play ((i + offset) / 2, i - 1, i);

        //n为奇数时，会出现一个内节点其孩子分别是内节点和外节点
        //先对这个内节点和外节点进行play() 
        if (n % 2 == 1) {
            _play (n / 2, temp[n - 1], lowExt + 1);
            i = lowExt + 3;
        } else {
            i = lowExt + 2;
        }

        //再对剩余的外部节点play()
        for (; i <= n; i += 2)
            _play ((i - lowExt + n - 1) / 2, i - 1, i);

        //将最终的赢者记录在tree[0]
        _tree_root[0] = temp[1];
    }
    
    //重拍 注意，切换元素在外部完成
    void reBuild (int thePlayer) {
        int n = _player_num;
        if (thePlayer <= 0 || thePlayer > n) {
            cout << "_players index is illgal" << endl;
            return;
        }

        //get the left _players and the right _players
        int matchPoint;     //左右选手的父亲节点
        int left, right;     //左右选手

        if (thePlayer <= lowExt) {       //the _players is on the bottom 
            matchPoint = (thePlayer + offset) / 2;
            left = 2 * matchPoint - offset;    // unify to the left _players
            right = left + 1;
        } else {   //the _players is on the last but one
            matchPoint = (thePlayer - lowExt + n - 1) / 2;
            //theplayer的左兄弟是最后一个内部节点
            if (2 * matchPoint == n - 1) {
                left = temp[2 * matchPoint];
                right = thePlayer;
            } else {
                left = 2 * matchPoint - (n - 1) + lowExt;
                right = left + 1;
            }
        }

        //重新比赛
        //重新比赛的选手在之前胜者的位置
        if (thePlayer == _tree_root[0]) {//the _players to reBuild is the previous winner!!!,ok,that's simple
            for (; matchPoint >= 1; matchPoint /= 2) {   //上次比赛的输者已经记录在tree[]中 
                int loserTemp = _getLose (_tree_root[matchPoint], thePlayer);
                temp[matchPoint] = _getWin (_tree_root[matchPoint], thePlayer);
                _tree_root[matchPoint] = loserTemp;
                thePlayer = temp[matchPoint];
            }
        } else {
            //否则从该节点到根的路径需重新比赛
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
        out << "成员内容：" << endl;
        for (int i = 0; i < _player_num; i++) {
            out << _players[_tree_root[i]-1] << " ";
        }
        out << endl;
        out << endl;

        out << "输者树结构：" << endl;
        //构造树
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
    int winner () { return temp[1]; }//获取胜者

    //int* theTree () { return _tree_root; }
private:
    int* _tree_root;       //内部节点，比赛的输者，tree[0]存放的是最终比赛的赢者
    int* temp;       //存放比赛的赢者
    T* _players;     //所要比赛的选手(外部节点)

    int _player_num; //选手的个数

    int lowExt;      //2*(n-s),最底层外部节点的个数,s=2^(log2(n-1))
                     //s为最底层最左端的内部节点的位置
    int offset;      //2*s-1

    int _getWin (int x, int y) { return _players[x] <= _players[y] ? x : y; }    //返回赢者
    int _getLose (int x, int y) { return _players[x] <= _players[y] ? y : x; }      //返回输者
    
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

