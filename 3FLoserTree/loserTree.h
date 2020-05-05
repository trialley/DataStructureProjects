#include<iostream>
#include<vector>
#include<map>
#pragma warning(disable:4996)

using namespace std;
template<class T>
class LoserTree {
public:
    LoserTree () { tree = nullptr; };
    LoserTree (T* thePlayers, int n) { initTree (thePlayers, n); }            //创建输者树
    ~LoserTree () { delete[] tree; delete[] winners; } //释放资源

    void initTree (T* thePlayers, int n) {
        auto _play = [this](int p, int left, int right) ->void {
            //根据player[l],_players[r]的值，设置tree[p]的值，即在左右子节点l，r之间举办比赛
            //之后如果p是右孩子节点，则继续举行上一层的比赛

            tree[p] = _whoLose (left, right);
            winners[p] = _whoWin (left, right);
            //内节点p是右孩子且非根
            while (p % 2 == 1 && p > 1) {
                tree[p / 2] = _whoLose (winners[p - 1], winners[p]);
                winners[p / 2] = _whoWin (winners[p - 1], winners[p]);

                p /= 2;
            }
        };
        tree = nullptr, winners = nullptr;

        if (n < 2) { return; }
        _players = thePlayers;
        _numOfPlayer = n;

        tree = new int[n];
        winners = new int[n];

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
            _play (n / 2, winners[n - 1], lowExt + 1);
            i = lowExt + 3;
        }
        else {
            i = lowExt + 2;
        }

        //再对剩余的外部节点play()
        for (; i <= n; i += 2)
            _play ((i - lowExt + n - 1) / 2, i - 1, i);

        //将最终的赢者记录在tree[0]
        tree[0] = winners[1];
    }

    //更新结构，thePlayer指向的元素已经被替换
    void replay (int thePlayer) {
        int n = _numOfPlayer;
        //判断player范围
        if (thePlayer <= 0 || thePlayer > n) {
            cout << "_players index is illgal" << endl;
            return;
        }

        //获取顺串新元素在树中的新位置
        int fatherPoint;//左右选手的父亲节点
        int left, right;//左右选手
        if (thePlayer <= lowExt) {//如果变化的节点在最底层
            fatherPoint = (thePlayer + offset) / 2;//获取父节点
            left = 2 * fatherPoint - offset;    // unify to the left _players
            right = left + 1;
        }
        else {   //the _players is on the last but one
            fatherPoint = (thePlayer - lowExt + n - 1) / 2;
            //theplayer的左兄弟是最后一个内部节点
            if (2 * fatherPoint == n - 1) {
                left = winners[2 * fatherPoint];
                right = thePlayer;
            }
            else {
                left = 2 * fatherPoint - (n - 1) + lowExt;
                right = left + 1;
            }
        }

        //重新比赛
        if (thePlayer == tree[0]) { //重新比赛的选手在之前胜者的位置
            for (; fatherPoint >= 1; fatherPoint /= 2) {   //上次比赛的输者已经记录在tree[]中 
                int loserTemp = _whoLose (tree[fatherPoint], thePlayer);//只需跟败者判断
                winners[fatherPoint] = _whoWin (tree[fatherPoint], thePlayer);
                tree[fatherPoint] = loserTemp;//父节点放置新的败者
                thePlayer = winners[fatherPoint];//赢者继续比赛
            }
        }else {//否则，无法判断新节点与老输者的关系，从该节点到根的路径需重新比赛
            tree[fatherPoint] = _whoLose (left, right);
            winners[fatherPoint] = _whoWin (left, right);

            //向上一级
            if (fatherPoint == n - 1 && n % 2 == 1) {
                fatherPoint /= 2;
                tree[fatherPoint] = _whoLose (winners[n - 1], lowExt + 1);
                winners[fatherPoint] = _whoWin (winners[n - 1], lowExt + 1);
            }

            fatherPoint /= 2;
            for (; fatherPoint >= 1; fatherPoint /= 2) {
                tree[fatherPoint] = _whoLose (winners[2 * fatherPoint], winners[2 * fatherPoint + 1]);
                winners[fatherPoint] = _whoWin (winners[2 * fatherPoint], winners[2 * fatherPoint + 1]);
            }
        }
        //保存最终胜者
        tree[0] = winners[1];
    }
    void output () const {
        for (int i = 0; i < _numOfPlayer; i++) {
            cout << _players[tree[i]] << endl;
        }
    }
    int* theTree () { return tree; }                //返回输者树
    int winner () { return winners[1]; }

private:
    int* tree;       //输者树内部节点
    int* winners;    //存放赢者

    T* _players;     //输者树外部节点
    int _numOfPlayer;//选手的个数

    int lowExt;      //2*(n-s),最底层外部节点的个数,s=2^(log2(n-1))
                     //s为最底层最左端的内部节点的位置
    int offset;      //2*s-1

    int _whoWin (int x, int y) { return _players[x] <= _players[y] ? x : y; }    //返回赢者
    int _whoLose (int x, int y) { return _players[x] <= _players[y] ? y : x; }      //返回输者
};

