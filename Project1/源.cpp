//�������ݽṹ: 
//�ýⷨ�ڱ�ʾ�����������ݽṹ�ϲ����˽ṹ������
//�ı�ʾ������ÿ���ڵ㰴������˳�����ṹ�����顣
//ÿ���ڵ�������������ڵ�Ϊ�ڽṹ�������еı�š�

//�㷨���̼�ע�� 

#include<iostream>
#include<sstream>

const int maxn = 200;

struct node {
	int left, right; //�����ӽڵ��ڽṹ�������е��±�� 
	int left_s, right_s; //���������ڵ��� 
	int v, id, ak;//v��ʾֵ��id��ʾ�����ӡʱ�ýڵ��ڵڼ��У�ak��ʾv�ĳ��� 
	char a[10];//���ڴ��v���ַ�����ʽ
}Nodes[maxn];

char map[maxn][maxn]; //���ڴ��Ҫ��ӡ�ĺ�������� 

using namespace std;

void BinaryTree_set (int rt, int k) { //���������������rt��ʾ���ڵ㣬k��ʾҪ����Ľڵ� 
	if (Nodes[k].v > Nodes[rt].v) { //����ڵ���ֵ���ڸ��ڵ���ֵʱ 
		Nodes[rt].right_s++; //���ڵ���������ڵ���������1 
		if (Nodes[rt].right != -1) { //�жϸ��ڵ���������Ƿ���� 
			BinaryTree_set (Nodes[rt].right, k); //�ݹ����������ĸ��ڵ�Ϊ���ڵ����k�ڵ� 
		} else { //������������
			Nodes[rt].right = k;//ֱ�ӽ���ǰ���ڵ��������Ϊk
		}
	} else {//ͬ��
		Nodes[rt].left_s++;
		if (Nodes[rt].left != -1) {
			BinaryTree_set (Nodes[rt].left, k);
		} else {
			Nodes[rt].left = k;
		}
	}
}

void Set_id (int rid, int k) { //������ڵ��ӡ�ڵڼ���  rid��ʾ��ǰ���ڵڼ��У�kΪҪ���м���Ľڵ� 
	Nodes[k].id = rid + Nodes[k].right_s + 1; //��֤��ӡ���в��ӵ�0�п�ʼ 
	if (Nodes[k].right != -1) {
		Set_id (rid, Nodes[k].right);//������������ʱ����ô�ͼ������������ڵ㴦�ڵڼ��У�
								  //��ʱridӦ��Ϊ������������rid����Ϊ�����������ȴ�ӡ��
								  //���������������ڵ�Ĳ�����Ϊ����������ڵ�Ĳ���������Ϊrid 
	}
	if (Nodes[k].left != -1) {
		Set_id (Nodes[k].id, Nodes[k].left); //������������������ڸ��ڵ��ӡ����
										//���Ĳ�����Ϊ���ڵ��ӡʱ���ڵ��� 
	}
}

void Map_set (int k, int index) { //����Ҫ��ӡ�ĺ��������������map�� k��ʾ����Ľڵ�ţ�
								//indexΪk�ڵ��map�ڼ��п�ʼ��� 
	for (int i = 0; i < Nodes[k].ak; i++) { //��k�ڵ��ֵ�����map�� 
		map[Nodes[k].id][index + i] = Nodes[k].a[Nodes[k].ak - i - 1];
	}
	index += Nodes[k].ak; //����index��ָ���ַ�����һλ 
	if (Nodes[k].right != -1 || Nodes[k].left != -1) { //��k�ڵ������Һ���ʱ 
		map[Nodes[k].id][index++] = '-';

		int max = Nodes[k].id;//���ڱ���k�ڵ��������������� 
		int min = Nodes[k].id; //���ڱ���k�ڵ��������������� 

		if (Nodes[k].right != -1) {//���������� 
			min = Nodes[Nodes[k].right].id; //�������������ڵ������ 
			map[Nodes[Nodes[k].right].id][index + 1] = '-';//�������������е�λ�÷���'-' 
			Map_set (Nodes[k].right, index + 2); //�ݹ�����ֿ�ʼ�������������ڵ㣬�����λ��Ϊindex+2 
		}
		if (Nodes[k].left != -1) {//ͬ�� 
			max = Nodes[Nodes[k].left].id;
			map[Nodes[Nodes[k].left].id][index + 1] = '-';
			Map_set (Nodes[k].left, index + 2);
		}
		map[min][index] = '/';
		map[max][index] = '\\';
		for (int i = min+1; i < max; i++) { //ͨ��min��max֪�������������ڵ��������min�е�max�з���'|' 
			map[i][index] = '|';
		}

		map[Nodes[k].id][index + 1] = '\0'; //�ýڵ������д�����󣬼�����β������ 
	} else {
		map[Nodes[k].id][index] = '\0';//�ýڵ������д�����󣬼�����β������ 
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
	while (ss >> e) { //��ȡ����ֵ 
		A[n++] = e;
	}

	for (int i = 0; i < n; i++) {
		int e = A[i];
		Nodes[i].right = Nodes[i].left = -1;
		Nodes[i].right_s = Nodes[i].left_s = 0;
		Nodes[i].v = e;
		Nodes[i].ak = 0;
		while (e) { //��vת�����ַ����浽a�У�ͬʱ�����ak 
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

