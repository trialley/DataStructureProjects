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
		//是森林
		//若初始为森林，每行的格式为 A B [nodes], 表示结点 A 拥有 B 个孩子结点, 孩子结点的集合为 nodes, i.e. 1 2 3 4 表示结点 1 拥有 2 个孩子, 分别为结点 3, 4。
		//for (int i = 0; i < N; i++) {
		//	//int a, b;
		//	//for (int j = 0; j < b; j++) {
		//	//	cin >> a >> b;

		//	//}
		//}
		f.initn (N);
	} else {
		//是树
	}

	return 0;
}
