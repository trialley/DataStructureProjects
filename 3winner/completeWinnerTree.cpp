/*
 * completeWinnerTree.cpp
 *
 *  Created on: 2019��7��14��
 *      Author: zyli
 */
#include "completeWinnerTree.h"
#include <limits.h>
#include <queue>

template<class T>
void completeWinnerTree<T>::initialize (T* thePlayer, int playersAmount, int flag) {// Ϊ���в����ߴ���Ӯ����

    int n = playersAmount;
    if (n < 2)
        throw std::out_of_range ("����Ҫ�����������ߣ�");

    player = thePlayer;
    numberOfPlayers = n;
    delete[] tree;
    tree = new int[n];
    //  s = 2^��log2(n-1)������ȡ��
    int i, s;
    for (s = 1; 2 * s <= n - 1; s *= 2);//s����ײ�����˵��ڲ��ڵ�

    lowExt = 2 * (n - s);//(n-s)����ײ��ڲ��ڵ������������ײ��ⲿ�ڵ����Ϊ2(n-s).
    offset = 2 * s - 1;//��ײ��ڲ��ڵ������е����һ��������һ���������ڼ���ÿ���ⲿ�ڵ�ĸ��ڲ��ڵ�ʱʹ�á�
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
    //   cout<<leftChild<<"��ѡ����"<<rightChild<<"��ѡ�ֱ�����Ӯ����"<<tree[p]<<"��ѡ�֡�"<<endl;
       // more matches possible if at right child
    while (p % 2 == 1 && p > 1) {// at a right child
        tree[p / 2] = (player[tree[p - 1]] <= player[tree[p]]) ?
            tree[p - 1] : tree[p];
        //      cout<<tree[p - 1]<<"��ѡ����"<<tree[p]<<"��ѡ�ֱ�����Ӯ����"<<tree[p / 2]<<"��ѡ�֡�"<<endl;
        p /= 2;  // go to parent

    }
}



template<class T>
void completeWinnerTree<T>::bigWinplay (int p, int leftChild, int rightChild) {// smallWinplay matches beginning at tree[p]
 // leftChild is left child of p
 // rightChild is right child of p

    tree[p] = (player[leftChild] >= player[rightChild]) ?
        leftChild : rightChild;
    cout << leftChild << "��ѡ����" << rightChild << "��ѡ�ֱ�����Ӯ����" << tree[p] << "��ѡ�֡�" << endl;
    // more matches possible if at right child
    while (p % 2 == 1 && p > 1) {// at a right child
        tree[p / 2] = (player[tree[p - 1]] >= player[tree[p]]) ?
            tree[p - 1] : tree[p];
        cout << tree[p - 1] << "��ѡ����" << tree[p] << "��ѡ�ֱ�����Ӯ����" << tree[p / 2] << "��ѡ�֡�" << endl;
        p /= 2;  // go to parent

    }
}


template<class T>
void completeWinnerTree<T>::smallWinRePlay (int thePlayer) {// Replay matches for player thePlayer.
//	cout<<"��ʼ���½�����"<<thePlayer<<"��ѡ���йصı���"<<endl;
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n)
        throw out_of_range ("Player index is illegal");

    int matchNode,       // ��Ҫ���¿�ʼ�����Ľڵ�
        leftChild,       // left child of matchNode
        rightChild;      // right child of matchNode

    // �ҵ���һ����Ҫ���¿�ʼ�����Ľڵ�����ĺ��ӽڵ�
    if (thePlayer <= lowExt) {// begin at lowest level
        matchNode = (offset + thePlayer) / 2;//��ײ��ⲿ�ڵ���㸸�ڵ㹫ʽ
        leftChild = 2 * matchNode - offset;//����ѡ�ֱ��
        rightChild = leftChild + 1;//�Һ���ѡ�ֱ��
    } else {
        matchNode = (thePlayer - lowExt + n - 1) / 2;//�����ⲿ�ڵ���㸸�ڵ㹫ʽ
        if (2 * matchNode == n - 1)//����������һ���ڲ��ڵ�
        {
            leftChild = tree[2 * matchNode];
            rightChild = thePlayer;
        } else//����������һ���ⲿ�ڵ�
        {
            leftChild = 2 * matchNode - (n - 1) + lowExt;//��2*matchNode�ҵ�matchNode��ڵ㣬Ȼ���ȥ�ڲ��ڵ������
            //�ҵ�����ڵ�֮ǰ���⵹���ڶ����м��������ߣ��ⲿ�ڵ㣩���ټ�����ײ�Ĳ������������ó����Ǹ����ӵĲ����߱�š�
            rightChild = leftChild + 1;//�����ڵ��Һ��ӵĲ����߱��
        }
    }

    tree[matchNode] = (player[leftChild] <= player[rightChild]) ? leftChild : rightChild;
    //   cout<<"��"<<thePlayer<<"��ѡ�ֽ���������"<<leftChild<<"��ѡ����"<<rightChild<<"��ѡ�ֽ������±�����Ӯ���ǣ�"<<tree[matchNode]<<"��ѡ��"<<endl;
       // special case for second match
    if (matchNode == n - 1 && n % 2 == 1) {
        matchNode /= 2;   // move to parent
        tree[matchNode] = (player[tree[n - 1]] <= player[lowExt + 1]) ?
            tree[n - 1] : lowExt + 1;
        //      cout<<tree[n-1]<<"��ѡ����"<<lowExt + 1<<"��ѡ�ֽ������±�����Ӯ���ǣ�"<<tree[matchNode]<<"��ѡ��"<<endl;
    }

    // smallWinplay remaining matches
    matchNode /= 2;
    for (; matchNode >= 1; matchNode /= 2) {// move to parent
        tree[matchNode] = (player[tree[2 * matchNode]] <= player[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        //   cout<<tree[2 * matchNode]<<"��ѡ����"<<tree[2 * matchNode + 1]<<"��ѡ�ֽ������±�����Ӯ���ǣ�"<<tree[matchNode]<<"��ѡ��"<<endl;
    }
}

template<class T>
void completeWinnerTree<T>::smallGenerateWinRePlay (int thePlayer) {// Replay matches for player thePlayer.
//	cout<<"��ʼ���½�����"<<thePlayer<<"��ѡ���йصı���"<<endl;
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n)
        throw out_of_range ("Player index is illegal");

    int matchNode,       // ��Ҫ���¿�ʼ�����Ľڵ�
        leftChild,       // left child of matchNode
        rightChild;      // right child of matchNode

    // �ҵ���һ����Ҫ���¿�ʼ�����Ľڵ�����ĺ��ӽڵ�
    if (thePlayer <= lowExt) {// begin at lowest level
        matchNode = (offset + thePlayer) / 2;//��ײ��ⲿ�ڵ���㸸�ڵ㹫ʽ
        leftChild = 2 * matchNode - offset;//����ѡ�ֱ��
        rightChild = leftChild + 1;//�Һ���ѡ�ֱ��
    } else {
        matchNode = (thePlayer - lowExt + n - 1) / 2;//�����ⲿ�ڵ���㸸�ڵ㹫ʽ
        if (2 * matchNode == n - 1)//����������һ���ڲ��ڵ�
        {
            leftChild = tree[2 * matchNode];
            rightChild = thePlayer;
        } else//����������һ���ⲿ�ڵ�
        {
            leftChild = 2 * matchNode - (n - 1) + lowExt;//��2*matchNode�ҵ�matchNode��ڵ㣬Ȼ���ȥ�ڲ��ڵ������
            //�ҵ�����ڵ�֮ǰ���⵹���ڶ����м��������ߣ��ⲿ�ڵ㣩���ټ�����ײ�Ĳ������������ó����Ǹ����ӵĲ����߱�š�
            rightChild = leftChild + 1;//�����ڵ��Һ��ӵĲ����߱��
        }
    }
    tree[matchNode] = (merge_num[leftChild] < merge_num[rightChild]) ? leftChild : rightChild;
    if (merge_num[leftChild] == merge_num[rightChild])
        tree[matchNode] = (player[leftChild] <= player[rightChild]) ? leftChild : rightChild;
    //   cout<<"��"<<thePlayer<<"��ѡ�ֽ���������"<<leftChild<<"��ѡ����"<<rightChild<<"��ѡ�ֽ������±�����Ӯ���ǣ�"<<tree[matchNode]<<"��ѡ��"<<endl;
       // special case for second match
    if (matchNode == n - 1 && n % 2 == 1) {
        matchNode /= 2;   // move to parent
        tree[matchNode] = (merge_num[tree[n - 1]] < merge_num[lowExt + 1]) ? tree[n - 1] : lowExt + 1;
        if (merge_num[tree[n - 1]] == merge_num[lowExt + 1])
            tree[matchNode] = (player[tree[n - 1]] <= player[lowExt + 1]) ? tree[n - 1] : lowExt + 1;
        //      cout<<tree[n-1]<<"��ѡ����"<<lowExt + 1<<"��ѡ�ֽ������±�����Ӯ���ǣ�"<<tree[matchNode]<<"��ѡ��"<<endl;
    }

    // smallWinplay remaining matches
    matchNode /= 2;
    for (; matchNode >= 1; matchNode /= 2) {// move to parent
        tree[matchNode] = (merge_num[tree[2 * matchNode]] < merge_num[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        if (merge_num[tree[2 * matchNode]] == merge_num[tree[2 * matchNode + 1]])
            tree[matchNode] = (player[tree[2 * matchNode]] <= player[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        //   cout<<tree[2 * matchNode]<<"��ѡ����"<<tree[2 * matchNode + 1]<<"��ѡ�ֽ������±�����Ӯ���ǣ�"<<tree[matchNode]<<"��ѡ��"<<endl;
    }
}

template<class T>
void completeWinnerTree<T>::bigWinRePlay (int thePlayer) {// Replay matches for player thePlayer.
    cout << "��ʼ���½�����" << thePlayer << "��ѡ���йصı���" << endl;
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n)
        throw out_of_range ("Player index is illegal");

    int matchNode,       // ��Ҫ���¿�ʼ�����Ľڵ�
        leftChild,       // left child of matchNode
        rightChild;      // right child of matchNode

    // �ҵ���һ����Ҫ���¿�ʼ�����Ľڵ�����ĺ��ӽڵ�
    if (thePlayer <= lowExt) {// begin at lowest level
        matchNode = (offset + thePlayer) / 2;//��ײ��ⲿ�ڵ���㸸�ڵ㹫ʽ
        leftChild = 2 * matchNode - offset;//����ѡ�ֱ��
        rightChild = leftChild + 1;//�Һ���ѡ�ֱ��
    } else {
        matchNode = (thePlayer - lowExt + n - 1) / 2;//�����ⲿ�ڵ���㸸�ڵ㹫ʽ
        if (2 * matchNode == n - 1)//����������һ���ڲ��ڵ�
        {
            leftChild = tree[2 * matchNode];
            rightChild = thePlayer;
        } else//����������һ���ⲿ�ڵ�
        {
            leftChild = 2 * matchNode - (n - 1) + lowExt;//��2*matchNode�ҵ�matchNode��ڵ㣬Ȼ���ȥ�ڲ��ڵ������
            //�ҵ�����ڵ�֮ǰ���⵹���ڶ����м��������ߣ��ⲿ�ڵ㣩���ټ�����ײ�Ĳ������������ó����Ǹ����ӵĲ����߱�š�
            rightChild = leftChild + 1;//�����ڵ��Һ��ӵĲ����߱��
        }
    }

    tree[matchNode] = (player[leftChild] >= player[rightChild]) ? leftChild : rightChild;
    cout << "��" << thePlayer << "��ѡ�ֽ���������" << leftChild << "��ѡ����" << rightChild << "��ѡ�ֽ������±�����Ӯ���ǣ�" << tree[matchNode] << "��ѡ��" << endl;
    // special case for second match
    if (matchNode == n - 1 && n % 2 == 1) {
        matchNode /= 2;   // move to parent
        tree[matchNode] = (player[tree[n - 1]] >= player[lowExt + 1]) ?
            tree[n - 1] : lowExt + 1;
        cout << tree[n - 1] << "��ѡ����" << lowExt + 1 << "��ѡ�ֽ������±�����Ӯ���ǣ�" << tree[matchNode] << "��ѡ��" << endl;
    }

    // smallWinplay remaining matches
    matchNode /= 2;  // move to parent
    for (; matchNode >= 1; matchNode /= 2) {
        tree[matchNode] = (player[tree[2 * matchNode]] >= player[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        cout << tree[2 * matchNode] << "��ѡ����" << tree[2 * matchNode + 1] << "��ѡ�ֽ������±�����Ӯ���ǣ�" << tree[matchNode] << "��ѡ��" << endl;
    }
}

template<class T>
void completeWinnerTree<T>::bigGenerateWinRePlay (int thePlayer) {// Replay matches for player thePlayer.
    cout << "��ʼ���½�����" << thePlayer << "��ѡ���йصı���" << endl;
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n)
        throw out_of_range ("Player index is illegal");

    int matchNode,       // ��Ҫ���¿�ʼ�����Ľڵ�
        leftChild,       // left child of matchNode
        rightChild;      // right child of matchNode

    // �ҵ���һ����Ҫ���¿�ʼ�����Ľڵ�����ĺ��ӽڵ�
    if (thePlayer <= lowExt) {// begin at lowest level
        matchNode = (offset + thePlayer) / 2;//��ײ��ⲿ�ڵ���㸸�ڵ㹫ʽ
        leftChild = 2 * matchNode - offset;//����ѡ�ֱ��
        rightChild = leftChild + 1;//�Һ���ѡ�ֱ��
    } else {
        matchNode = (thePlayer - lowExt + n - 1) / 2;//�����ⲿ�ڵ���㸸�ڵ㹫ʽ
        if (2 * matchNode == n - 1)//����������һ���ڲ��ڵ�
        {
            leftChild = tree[2 * matchNode];
            rightChild = thePlayer;
        } else//����������һ���ⲿ�ڵ�
        {
            leftChild = 2 * matchNode - (n - 1) + lowExt;//��2*matchNode�ҵ�matchNode��ڵ㣬Ȼ���ȥ�ڲ��ڵ������
            //�ҵ�����ڵ�֮ǰ���⵹���ڶ����м��������ߣ��ⲿ�ڵ㣩���ټ�����ײ�Ĳ������������ó����Ǹ����ӵĲ����߱�š�
            rightChild = leftChild + 1;//�����ڵ��Һ��ӵĲ����߱��
        }
    }
    tree[matchNode] = (merge_num[leftChild] < merge_num[rightChild]) ? leftChild : rightChild;
    if (merge_num[leftChild] == merge_num[rightChild])
        tree[matchNode] = (player[leftChild] >= player[rightChild]) ? leftChild : rightChild;
    cout << "��" << thePlayer << "��ѡ�ֽ���������" << leftChild << "��ѡ����" << rightChild << "��ѡ�ֽ������±�����Ӯ���ǣ�" << tree[matchNode] << "��ѡ��" << endl;
    // special case for second match
    if (matchNode == n - 1 && n % 2 == 1) {
        matchNode /= 2;   // move to parent
        tree[matchNode] = (merge_num[tree[n - 1]] < merge_num[lowExt + 1]) ? tree[n - 1] : lowExt + 1;
        if (merge_num[tree[n - 1]] == merge_num[lowExt + 1])
            tree[matchNode] = (player[tree[n - 1]] >= player[lowExt + 1]) ? tree[n - 1] : lowExt + 1;
        cout << tree[n - 1] << "��ѡ����" << lowExt + 1 << "��ѡ�ֽ������±�����Ӯ���ǣ�" << tree[matchNode] << "��ѡ��" << endl;
    }

    // smallWinplay remaining matches
    matchNode /= 2;
    for (; matchNode >= 1; matchNode /= 2) {// move to parent
        tree[matchNode] = (merge_num[tree[2 * matchNode]] < merge_num[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        if (merge_num[tree[2 * matchNode]] == merge_num[tree[2 * matchNode + 1]])
            tree[matchNode] = (player[tree[2 * matchNode]] >= player[tree[2 * matchNode + 1]]) ?
            tree[2 * matchNode] : tree[2 * matchNode + 1];
        cout << tree[2 * matchNode] << "��ѡ����" << tree[2 * matchNode + 1] << "��ѡ�ֽ������±�����Ӯ���ǣ�" << tree[matchNode] << "��ѡ��" << endl;
    }
}

template<class T>
void completeWinnerTree<T>::output () const {
    cout << "����������  = " << numberOfPlayers
        << " ,��������= " << numberOfPlayers - 1
        << " ,��ײ��ⲿ�ڵ����lowExt = " << lowExt << endl;
    //        << " ��ײ��ڲ��ڵ������е����һ������offset = " << offset << endl;
    cout << ",Ӯ��id��Ӯ�����д��ϵ��£�������˳��Ϊ��" << endl;
    for (int i = 1; i < numberOfPlayers; i++)
        cout << tree[i] << ' ';
    cout << endl;
    cout << "����Ӯ��Ϊ��" << winner () << "��ѡ��" << endl;
}
template<class T>
void completeWinnerTree<T>::Kway_mergerwithWT (T* array, int arrayLenth, int p, int flag) {
    /*****************************
     * 							 *
      *          ��Ӯ������ʼ���鲢��       *
     *							 *
     ****************************/
     //	if(p >50000) throw std::invalid_argument("k��2p should be <=1000");
    merge_num = new int[p + 1];//��Ӧ�Ĺ鲢��
    for (int i = 1; i <= p; i++) merge_num[i] = 1;//ȫ����ʼ��Ϊ1

    T* arraytemple = new T[p + 1];//���ڳ�ʼ��Ӯ������������ʼ���ֵ������
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

    R[1].push (player[winner ()]);//����ʼ�����һ������Ӯ�ߣ�һ���ᣩ����R1����



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
      *     ��Ӯ����k·�鲢����          *
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
    }//�����й鲢�εĵ�һ��Ԫ���ø�Ӯ������ʼ��ѡ��

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
    cout << "�����Ĺ鲢·��Ϊ��" << mergeNum << endl;

}

