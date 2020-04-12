/*
 * completeWinnerTree.cpp
 *
 *  Created on: 2019年7月14日
 *      Author: zyli
 */
#include "completeWinnerTree.h"
#include <limits.h>
#include <queue>

template<class T>
void completeWinnerTree<T>::initialize (T* thePlayer, int playersAmount, int flag) {// 为所有参赛者创建赢者树

    int n = playersAmount;
    if (n < 2)
        throw std::out_of_range ("至少要有两名参赛者！");

    player = thePlayer;
    numberOfPlayers = n;
    delete[] tree;
    tree = new int[n];
    //  s = 2^（log2(n-1)）向下取整
    int i, s;
    for (s = 1; 2 * s <= n - 1; s *= 2);//s是最底层最左端的内部节点

    lowExt = 2 * (n - s);//(n-s)是最底层内部节点个数，所以最底层外部节点个数为2(n-s).
    offset = 2 * s - 1;//最底层内部节点所在行的最后一个参数，一个参量，在计算每个外部节点的父内部节点时使用。
    if (flag == 1) {
        // smallWinplay matches for lowest-level external nodes
        for (i = 2; i <= lowExt; i += 2) {
            smallWinplay ((offset + i) / 2, i - 1, i);
        }
        // handle remaining external nodes
        if (n % 2 == 1) {// special case for odd n, smallWinplay internal and exteral node
            smallWinplay (n / 2, tree[n - 1], lowExt + 1);
            i = lowExt + 3;
        } else i = lowExt + 2;

        // i is left-most remaining external node
        for (; i <= n; i += 2)
            smallWinplay ((i - lowExt + n - 1) / 2, i - 1, i);
    }
    if (flag == 2) {
        // bigWinplay matches for lowest-level external nodes
        for (i = 2; i <= lowExt; i += 2) {
            bigWinplay ((offset + i) / 2, i - 1, i);
        }
        // handle remaining external nodes
        if (n % 2 == 1) {// special case for odd n, bigWinplay internal and exteral node
            bigWinplay (n / 2, tree[n - 1], lowExt + 1);
            i = lowExt + 3;
        } else i = lowExt + 2;

        // i is left-most remaining external node
        for (; i <= n; i += 2)
            bigWinplay ((i - lowExt + n - 1) / 2, i - 1, i);
    }
    if (flag == 3) {
        // smallGeneralWinplay matches for lowest-level external nodes
        for (i = 2; i <= lowExt; i += 2) {
            smallWinplay ((offset + i) / 2, i - 1, i);
        }
        // handle remaining external nodes
        if (n % 2 == 1) {// special case for odd n, bigWinplay internal and exteral node
            smallWinplay (n / 2, tree[n - 1], lowExt + 1);
            i = lowExt + 3;
        } else i = lowExt + 2;

        // i is left-most remaining external node
        for (; i <= n; i += 2)
            smallWinplay ((i - lowExt + n - 1) / 2, i - 1, i);
    }
    if (flag == 4) {
        // smallGeneralWinplay matches for lowest-level external nodes
        for (i = 2; i <= lowExt; i += 2) {
            bigWinplay ((offset + i) / 2, i - 1, i);
        }
        // handle remaining external nodes
        if (n % 2 == 1) {// special case for odd n, bigWinplay internal and exteral node
            bigWinplay (n / 2, tree[n - 1], lowExt + 1);
            i = lowExt + 3;
        } else i = lowExt + 2;

        // i is left-most remaining external node
        for (; i <= n; i += 2)
            bigWinplay ((i - lowExt + n - 1) / 2, i - 1, i);
    }
}

template<class T>
void completeWinnerTree<T>::smallWinplay (int p, int leftChild, int rightChild) {// smallWinplay matches beginning at tree[p]
 // leftChild is left child of p
 // rightChild is right child of p

    tree[p] = (player[leftChild] <= player[rightChild]) ?
        leftChild : rightChild;
    //   cout<<leftChild<<"号选手与"<<rightChild<<"号选手比赛，赢者是"<<tree[p]<<"号选手。"<<endl;
       // more matches possible if at right child
    while (p % 2 == 1 && p > 1) {// at a right child
        tree[p / 2] = (player[tree[p - 1]] <= player[tree[p]]) ?
            tree[p - 1] : tree[p];
        //      cout<<tree[p - 1]<<"号选手与"<<tree[p]<<"号选手比赛，赢者是"<<tree[p / 2]<<"号选手。"<<endl;
        p /= 2;  // go to parent

    }
}



template<class T>
void completeWinnerTree<T>::bigWinplay (int p, int leftChild, int rightChild) {// smallWinplay matches beginning at tree[p]
 // leftChild is left child of p
 // rightChild is right child of p

    tree[p] = (player[leftChild] >= player[rightChild]) ?
        leftChild : rightChild;
    cout << leftChild << "号选手与" << rightChild << "号选手比赛，赢者是" << tree[p] << "号选手。" << endl;
    // more matches possible if at right child
    while (p % 2 == 1 && p > 1) {// at a right child
        tree[p / 2] = (player[tree[p - 1]] >= player[tree[p]]) ?
            tree[p - 1] : tree[p];
        cout << tree[p - 1] << "号选手与" << tree[p] << "号选手比赛，赢者是" << tree[p / 2] << "号选手。" << endl;
        p /= 2;  // go to parent

    }
}


template<class T>
void completeWinnerTree<T>::smallWinRePlay (int thePlayer) {// Replay matches for player thePlayer.
//	cout<<"开始重新进行与"<<thePlayer<<"号选手有关的比赛"<<endl;
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n)
        throw out_of_range ("Player index is illegal");

    int matchNode,       // 需要重新开始比赛的节点
        leftChild,       // left child of matchNode
        rightChild;      // right child of matchNode

    // 找到第一个需要重新开始比赛的节点和他的孩子节点
    if (thePlayer <= lowExt) {// begin at lowest level
        matchNode = (offset + thePlayer) / 2;//最底层外部节点计算父节点公式
        leftChild = 2 * matchNode - offset;//左孩子选手编号
        rightChild = leftChild + 1;//右孩子选手编号
    } else {
        matchNode = (thePlayer - lowExt + n - 1) / 2;//其他外部节点计算父节点公式
        if (2 * matchNode == n - 1)//它的左孩子是一个内部节点
        {
            leftChild = tree[2 * matchNode];
            rightChild = thePlayer;
        } else//它的左孩子是一个外部节点
        {
            leftChild = 2 * matchNode - (n - 1) + lowExt;//先2*matchNode找到matchNode左节点，然后减去内部节点个数，
            //找到该左节点之前在这倒数第二层有几个参赛者（外部节点），再加上最底层的参赛者人数，得出的是该左孩子的参赛者编号。
            rightChild = leftChild + 1;//比赛节点右孩子的参赛者编号
        }
    }

    tree[matchNode] = (player[leftChild] <= player[rightChild]) ? leftChild : rightChild;
    //   cout<<"从"<<thePlayer<<"号选手进行重赛，"<<leftChild<<"号选手与"<<rightChild<<"号选手进行重新比赛，赢者是："<<tree[matchNode]<<"号选手"<<endl;
       // special case for second match
    if (matchNode == n - 1 && n % 2 == 1) {
        matchNode /= 2;   // move to parent
        tree[matchNode] = (player[tree[n - 1]] <= player[lowExt + 1]) ?
            tree[n - 1] : lowExt + 1;
        //      cout<<tree[n-1]<<"号选手与"<<lowExt + 1<<"号选手进行重新比赛，赢者是："<<tree[matchNode]<<"号选手"<<endl;
    }

    // smallWinplay remaining matches
    matchNode /= 2;
    for (; matchNode >= 1; matchNode /= 2) {// move to parent
        tree[matchNode] = (player[tree[2 * matchNode]] <= player[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        //   cout<<tree[2 * matchNode]<<"号选手与"<<tree[2 * matchNode + 1]<<"号选手进行重新比赛，赢者是："<<tree[matchNode]<<"号选手"<<endl;
    }
}

template<class T>
void completeWinnerTree<T>::smallGenerateWinRePlay (int thePlayer) {// Replay matches for player thePlayer.
//	cout<<"开始重新进行与"<<thePlayer<<"号选手有关的比赛"<<endl;
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n)
        throw out_of_range ("Player index is illegal");

    int matchNode,       // 需要重新开始比赛的节点
        leftChild,       // left child of matchNode
        rightChild;      // right child of matchNode

    // 找到第一个需要重新开始比赛的节点和他的孩子节点
    if (thePlayer <= lowExt) {// begin at lowest level
        matchNode = (offset + thePlayer) / 2;//最底层外部节点计算父节点公式
        leftChild = 2 * matchNode - offset;//左孩子选手编号
        rightChild = leftChild + 1;//右孩子选手编号
    } else {
        matchNode = (thePlayer - lowExt + n - 1) / 2;//其他外部节点计算父节点公式
        if (2 * matchNode == n - 1)//它的左孩子是一个内部节点
        {
            leftChild = tree[2 * matchNode];
            rightChild = thePlayer;
        } else//它的左孩子是一个外部节点
        {
            leftChild = 2 * matchNode - (n - 1) + lowExt;//先2*matchNode找到matchNode左节点，然后减去内部节点个数，
            //找到该左节点之前在这倒数第二层有几个参赛者（外部节点），再加上最底层的参赛者人数，得出的是该左孩子的参赛者编号。
            rightChild = leftChild + 1;//比赛节点右孩子的参赛者编号
        }
    }
    tree[matchNode] = (merge_num[leftChild] < merge_num[rightChild]) ? leftChild : rightChild;
    if (merge_num[leftChild] == merge_num[rightChild])
        tree[matchNode] = (player[leftChild] <= player[rightChild]) ? leftChild : rightChild;
    //   cout<<"从"<<thePlayer<<"号选手进行重赛，"<<leftChild<<"号选手与"<<rightChild<<"号选手进行重新比赛，赢者是："<<tree[matchNode]<<"号选手"<<endl;
       // special case for second match
    if (matchNode == n - 1 && n % 2 == 1) {
        matchNode /= 2;   // move to parent
        tree[matchNode] = (merge_num[tree[n - 1]] < merge_num[lowExt + 1]) ? tree[n - 1] : lowExt + 1;
        if (merge_num[tree[n - 1]] == merge_num[lowExt + 1])
            tree[matchNode] = (player[tree[n - 1]] <= player[lowExt + 1]) ? tree[n - 1] : lowExt + 1;
        //      cout<<tree[n-1]<<"号选手与"<<lowExt + 1<<"号选手进行重新比赛，赢者是："<<tree[matchNode]<<"号选手"<<endl;
    }

    // smallWinplay remaining matches
    matchNode /= 2;
    for (; matchNode >= 1; matchNode /= 2) {// move to parent
        tree[matchNode] = (merge_num[tree[2 * matchNode]] < merge_num[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        if (merge_num[tree[2 * matchNode]] == merge_num[tree[2 * matchNode + 1]])
            tree[matchNode] = (player[tree[2 * matchNode]] <= player[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        //   cout<<tree[2 * matchNode]<<"号选手与"<<tree[2 * matchNode + 1]<<"号选手进行重新比赛，赢者是："<<tree[matchNode]<<"号选手"<<endl;
    }
}

template<class T>
void completeWinnerTree<T>::bigWinRePlay (int thePlayer) {// Replay matches for player thePlayer.
    cout << "开始重新进行与" << thePlayer << "号选手有关的比赛" << endl;
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n)
        throw out_of_range ("Player index is illegal");

    int matchNode,       // 需要重新开始比赛的节点
        leftChild,       // left child of matchNode
        rightChild;      // right child of matchNode

    // 找到第一个需要重新开始比赛的节点和他的孩子节点
    if (thePlayer <= lowExt) {// begin at lowest level
        matchNode = (offset + thePlayer) / 2;//最底层外部节点计算父节点公式
        leftChild = 2 * matchNode - offset;//左孩子选手编号
        rightChild = leftChild + 1;//右孩子选手编号
    } else {
        matchNode = (thePlayer - lowExt + n - 1) / 2;//其他外部节点计算父节点公式
        if (2 * matchNode == n - 1)//它的左孩子是一个内部节点
        {
            leftChild = tree[2 * matchNode];
            rightChild = thePlayer;
        } else//它的左孩子是一个外部节点
        {
            leftChild = 2 * matchNode - (n - 1) + lowExt;//先2*matchNode找到matchNode左节点，然后减去内部节点个数，
            //找到该左节点之前在这倒数第二层有几个参赛者（外部节点），再加上最底层的参赛者人数，得出的是该左孩子的参赛者编号。
            rightChild = leftChild + 1;//比赛节点右孩子的参赛者编号
        }
    }

    tree[matchNode] = (player[leftChild] >= player[rightChild]) ? leftChild : rightChild;
    cout << "从" << thePlayer << "号选手进行重赛，" << leftChild << "号选手与" << rightChild << "号选手进行重新比赛，赢者是：" << tree[matchNode] << "号选手" << endl;
    // special case for second match
    if (matchNode == n - 1 && n % 2 == 1) {
        matchNode /= 2;   // move to parent
        tree[matchNode] = (player[tree[n - 1]] >= player[lowExt + 1]) ?
            tree[n - 1] : lowExt + 1;
        cout << tree[n - 1] << "号选手与" << lowExt + 1 << "号选手进行重新比赛，赢者是：" << tree[matchNode] << "号选手" << endl;
    }

    // smallWinplay remaining matches
    matchNode /= 2;  // move to parent
    for (; matchNode >= 1; matchNode /= 2) {
        tree[matchNode] = (player[tree[2 * matchNode]] >= player[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        cout << tree[2 * matchNode] << "号选手与" << tree[2 * matchNode + 1] << "号选手进行重新比赛，赢者是：" << tree[matchNode] << "号选手" << endl;
    }
}

template<class T>
void completeWinnerTree<T>::bigGenerateWinRePlay (int thePlayer) {// Replay matches for player thePlayer.
    cout << "开始重新进行与" << thePlayer << "号选手有关的比赛" << endl;
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n)
        throw out_of_range ("Player index is illegal");

    int matchNode,       // 需要重新开始比赛的节点
        leftChild,       // left child of matchNode
        rightChild;      // right child of matchNode

    // 找到第一个需要重新开始比赛的节点和他的孩子节点
    if (thePlayer <= lowExt) {// begin at lowest level
        matchNode = (offset + thePlayer) / 2;//最底层外部节点计算父节点公式
        leftChild = 2 * matchNode - offset;//左孩子选手编号
        rightChild = leftChild + 1;//右孩子选手编号
    } else {
        matchNode = (thePlayer - lowExt + n - 1) / 2;//其他外部节点计算父节点公式
        if (2 * matchNode == n - 1)//它的左孩子是一个内部节点
        {
            leftChild = tree[2 * matchNode];
            rightChild = thePlayer;
        } else//它的左孩子是一个外部节点
        {
            leftChild = 2 * matchNode - (n - 1) + lowExt;//先2*matchNode找到matchNode左节点，然后减去内部节点个数，
            //找到该左节点之前在这倒数第二层有几个参赛者（外部节点），再加上最底层的参赛者人数，得出的是该左孩子的参赛者编号。
            rightChild = leftChild + 1;//比赛节点右孩子的参赛者编号
        }
    }
    tree[matchNode] = (merge_num[leftChild] < merge_num[rightChild]) ? leftChild : rightChild;
    if (merge_num[leftChild] == merge_num[rightChild])
        tree[matchNode] = (player[leftChild] >= player[rightChild]) ? leftChild : rightChild;
    cout << "从" << thePlayer << "号选手进行重赛，" << leftChild << "号选手与" << rightChild << "号选手进行重新比赛，赢者是：" << tree[matchNode] << "号选手" << endl;
    // special case for second match
    if (matchNode == n - 1 && n % 2 == 1) {
        matchNode /= 2;   // move to parent
        tree[matchNode] = (merge_num[tree[n - 1]] < merge_num[lowExt + 1]) ? tree[n - 1] : lowExt + 1;
        if (merge_num[tree[n - 1]] == merge_num[lowExt + 1])
            tree[matchNode] = (player[tree[n - 1]] >= player[lowExt + 1]) ? tree[n - 1] : lowExt + 1;
        cout << tree[n - 1] << "号选手与" << lowExt + 1 << "号选手进行重新比赛，赢者是：" << tree[matchNode] << "号选手" << endl;
    }

    // smallWinplay remaining matches
    matchNode /= 2;
    for (; matchNode >= 1; matchNode /= 2) {// move to parent
        tree[matchNode] = (merge_num[tree[2 * matchNode]] < merge_num[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        if (merge_num[tree[2 * matchNode]] == merge_num[tree[2 * matchNode + 1]])
            tree[matchNode] = (player[tree[2 * matchNode]] >= player[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        cout << tree[2 * matchNode] << "号选手与" << tree[2 * matchNode + 1] << "号选手进行重新比赛，赢者是：" << tree[matchNode] << "号选手" << endl;
    }
}

template<class T>
void completeWinnerTree<T>::output () const {
    cout << "竞赛者人数  = " << numberOfPlayers
        << " ,比赛轮数= " << numberOfPlayers - 1
        << " ,最底层外部节点个数lowExt = " << lowExt << endl;
    //        << " 最底层内部节点所在行的最后一个参数offset = " << offset << endl;
    cout << ",赢者id按赢者树中从上到下，从左到右顺序为：" << endl;
    for (int i = 1; i < numberOfPlayers; i++)
        cout << tree[i] << ' ';
    cout << endl;
    cout << "最终赢者为：" << winner () << "号选手" << endl;
}
template<class T>
void completeWinnerTree<T>::Kway_mergerwithWT (T* array, int arrayLenth, int p, int flag) {
    /*****************************
     * 							 *
      *          用赢者树初始化归并段       *
     *							 *
     ****************************/
     //	if(p >50000) throw std::invalid_argument("k≈2p should be <=1000");
    merge_num = new int[p + 1];//对应的归并号
    for (int i = 1; i <= p; i++) merge_num[i] = 1;//全部初始化为1

    T* arraytemple = new T[p + 1];//用于初始化赢者树，赋给初始玩家值的数组
    if (arrayLenth < p) {
        for (int i = 1; i <= arrayLenth; i++)
            arraytemple[i] = array[i - 1];
        for (int i = arrayLenth + 1; i <= p; i++)
            arraytemple[i] = INT_MAX;
    } else {
        for (int i = 1; i <= p; i++) { arraytemple[i] = array[i - 1]; }
    }
    initialize (arraytemple, p, 1);

    queue<T>* R = new queue<T>[2 * p + 1];

    R[1].push (player[winner ()]);//将初始化完第一个最终赢者（一定会）放入R1数组



    for (int i = p; i < arrayLenth; i++) {
        if (array[i] < player[winner ()]) {
            merge_num[winner ()]++;
            player[winner ()] = array[i];
            smallGenerateWinRePlay (winner ());

        } else {
            player[winner ()] = array[i];
            smallGenerateWinRePlay (winner ());
        }

        R[merge_num[winner ()]].push (player[winner ()]);


    }

    int mergeNum = 0;

    for (int i = 1; i <= p; i++) if (merge_num[i] > mergeNum) mergeNum = merge_num[i];


    for (int count = 0; count < p - 1; count++) {

        merge_num[winner ()] = INT_MAX;
        smallGenerateWinRePlay (winner ());
        R[merge_num[winner ()]].push (player[winner ()]);


    }


    /*****************************
     * 							 *
      *     用赢者树k路归并排序          *
     *							 *
     ****************************/




    T* arraytemple2 = new T[mergeNum + 1];
    if (mergeNum == 1) {
        for (int i = 0; i < arrayLenth; i++) { 
            array[i] = R[mergeNum].front ();
            R[mergeNum].pop (); 
        }
        return;
    }

    for (int i = 1; i <= mergeNum; i++) { 
        arraytemple2[i] = R[i].front ();
        R[i].pop ();
    }//把所有归并段的第一个元素拿给赢者树初始化选手

    initialize (arraytemple2, mergeNum, 1);

    array[0] = player[winner ()];


    int i = 1;
    while (player[winner ()] != INT_MAX) {

        if (!R[winner ()].empty ()) {
            player[winner ()] = R[winner ()].front ();
            R[winner ()].pop ();
        } else
            player[winner ()] = INT_MAX;

        smallWinRePlay (winner ());
        array[i] = player[winner ()];
        i++;

    }
    cout << "产生的归并路数为：" << mergeNum << endl;

}

