#include "BinaryTree.h"
#include "Tree.h"
#include"Forest.h"
#include <iostream>
#include <vector>
using namespace std;

#define DEBUG
#ifdef DEBUG
#define dprintf printf
#else
#define dprintf  /\
/printf
#endif

int main () {
	freopen ("in1.txt", "r", stdin);
	Forest<int> f;
	BTree<int> t;
	int K, N, M;
	cin >> K, N, M;

	if (K == 0) {
		//��ɭ��
		//����ʼΪɭ�֣�ÿ�еĸ�ʽΪ A B [nodes], ��ʾ��� A ӵ�� B �����ӽ��, ���ӽ��ļ���Ϊ nodes, i.e. 1 2 3 4 ��ʾ��� 1 ӵ�� 2 ������, �ֱ�Ϊ��� 3, 4��
		//for (int i = 0; i < N; i++) {
		//	//int a, b;
		//	//for (int j = 0; j < b; j++) {
		//	//	cin >> a >> b;

		//	//}
		//}
		f.initn (N);
	} else {
		//����
	}

	return 0;
}
