//所用数据结构: 
//该解法在表示二叉树的数据结构上采用了结构体数组
//的表示方法，每个节点按照输入顺序存入结构体数组。
//每个节点的左右子树根节点为在结构体数组中的编号。

//算法过程见注释 

#include<iostream>
#include<sstream>

const int maxn = 200;

struct node {
	int left, right; //左右子节点在结构体数组中的下标号 
	int left_s, right_s; //左右子树节点数 
	int v, id, ak;//v表示值，id表示横向打印时该节点在第几行，ak表示v的长度 
	char a[10];//用于存放v的字符串形式
}Nodes[maxn];

char map[maxn][maxn]; //用于存放要打印的横向二叉树 

using namespace std;

void BinaryTree_set (int rt, int k) { //构建排序二叉树，rt表示根节点，k表示要插入的节点 
	if (Nodes[k].v > Nodes[rt].v) { //插入节点数值大于根节点数值时 
		Nodes[rt].right_s++; //根节点的右子树节点数会增加1 
		if (Nodes[rt].right != -1) { //判断根节点的右子树是否存在 
			BinaryTree_set (Nodes[rt].right, k); //递归以右子树的根节点为根节点插入k节点 
		} else { //右子树不存在
			Nodes[rt].right = k;//直接将当前根节点的右子树为k
		}
	} else {//同理
		Nodes[rt].left_s++;
		if (Nodes[rt].left != -1) {
			BinaryTree_set (Nodes[rt].left, k);
		} else {
			Nodes[rt].left = k;
		}
	}
}

void Set_id (int rid, int k) { //计算各节点打印在第几行  rid表示当前处于第几行，k为要进行计算的节点 
	Nodes[k].id = rid + Nodes[k].right_s + 1; //保证打印的行不从第0行开始 
	if (Nodes[k].right != -1) {
		Set_id (rid, Nodes[k].right);//当右子树存在时，那么就计算右子树根节点处于第几行，
								  //此时rid应该为参数传进来的rid，因为右子树会优先打印，
								  //右子树是以他父节点的参照物为参照物，他父节点的参照物正好为rid 
	}
	if (Nodes[k].left != -1) {
		Set_id (Nodes[k].id, Nodes[k].left); //由于是左子树，相对于父节点打印的晚，
										//他的参照物为父节点打印时所在的行 
	}
}

void Map_set (int k, int index) { //构建要打印的横向二叉树，存在map中 k表示处理的节点号，
								//index为k节点从map第几列开始存放 
	for (int i = 0; i < Nodes[k].ak; i++) { //把k节点的值存放在map中 
		map[Nodes[k].id][index + i] = Nodes[k].a[Nodes[k].ak - i - 1];
	}
	index += Nodes[k].ak; //更新index，指向字符串后一位 
	if (Nodes[k].right != -1 || Nodes[k].left != -1) { //当k节点有左右孩子时 
		map[Nodes[k].id][index++] = '-';

		int max = Nodes[k].id;//用于保存k节点左子树所在行数 
		int min = Nodes[k].id; //用于保存k节点右子树所在行数 

		if (Nodes[k].right != -1) {//右子树存在 
			min = Nodes[Nodes[k].right].id; //保存右子树根节点的行数 
			map[Nodes[Nodes[k].right].id][index + 1] = '-';//在右子树所在行的位置放入'-' 
			Map_set (Nodes[k].right, index + 2); //递归进行又开始处理右子树根节点，其存入位置为index+2 
		}
		if (Nodes[k].left != -1) {//同理 
			max = Nodes[Nodes[k].left].id;
			map[Nodes[Nodes[k].left].id][index + 1] = '-';
			Map_set (Nodes[k].left, index + 2);
		}
		map[min][index] = '/';
		map[max][index] = '\\';
		for (int i = min+1; i < max; i++) { //通过min和max知道左右子树根节点的行数，min行到max行放入'|' 
			map[i][index] = '|';
		}

		map[Nodes[k].id][index + 1] = '\0'; //该节点所在行处理完后，加入行尾结束符 
	} else {
		map[Nodes[k].id][index] = '\0';//该节点所在行处理完后，加入行尾结束符 
		return;
	}
}

int main () {
	string s;
	getline (cin, s);
	istringstream ss (s);
	int e;
	int n = 0;
	int A[maxn];
	while (ss >> e) { //读取输入值 
		A[n++] = e;
	}

	for (int i = 0; i < n; i++) {
		int e = A[i];
		Nodes[i].right = Nodes[i].left = -1;
		Nodes[i].right_s = Nodes[i].left_s = 0;
		Nodes[i].v = e;
		Nodes[i].ak = 0;
		while (e) { //把v转换成字符串存到a中，同时计算出ak 
			Nodes[i].a[Nodes[i].ak++] = e % 10 + '0';
			e = e / 10;
		}
	}
	for (int i = 1; i < n; i++) {
		BinaryTree_set (0, i);
	}
	Set_id (0, 0);
	for (int i = 1; i < maxn; i++) {
		for (int j = 0; j < maxn; j++) {
			map[i][j] = ' ';
		}
	}
	Map_set (0, 0);
	for (int i = 1; i <= n; i++) {
		cout << map[i] << endl;
	}
	return 0;
}

