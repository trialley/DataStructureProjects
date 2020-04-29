#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template <typename T>
class Node {
public:

	Node () : _index (0), _parent (0), _left (0), _right (0), _firstChild (0), _sibling (0), _data (0) {}
	Node (int index, T data) : _index (index), _data (data), _parent (0), _left (0), _right (0), _firstChild (0), _sibling (0) {}
	~Node () {
		if (_left != nullptr) {
			_left->BTreeDeleteAll ();
			_left = nullptr;
		}

		if (_right != nullptr) {
			_right->BTreeDeleteAll ();
			_right = nullptr;
		}

		if (_firstChild != nullptr) {
			_firstChild->NodeDelete ();
			_firstChild = nullptr;
		}

		if (_sibling != nullptr) {
			_sibling->NodeDelete ();
			_sibling = nullptr;
		}
		_parent = nullptr;
	}

	//get data
	int getIndex () { return _index; }
	T getData () { return _data; }
	Node<T>* getParent () { return _parent; }
	Node<T>* getLChild () { return _left; }
	Node<T>* getRChild () { return _right; }
	Node<T>* getFirstChild () { return _firstChild; }
	Node<T>* getNextSibling () { return _sibling; }

	void setIndex (int index) { _index = index; }
	void setData (T data) { _data = data; }
	void setParenet (Node* Node) { _parent = Node; }
	void setLChild (Node* Node) {
		_left = Node;
		if (Node != nullptr) Node->setParenet (this);
	}
	void setRChild (Node* Node) {
		_right = Node;
		if (Node != nullptr) Node->setParenet (this);
	}
	void setFirstChild (Node* Node) { _firstChild = Node; }
	void setNextSibling (Node* Node) { _sibling = Node; }
	void setChild (Node* nodein) {	 //�޺��ӵ����
		if (_firstChild == nullptr) {
			_firstChild = nodein;
			nodein->setParenet (this);
		}
		//��һ�����ӵ�����ֵ����Node������ֵ�����
		else if (nodein->getIndex () < _firstChild->getIndex ()) {
			nodein->setParenet (this);
			nodein->setNextSibling (_firstChild);
			_firstChild = nodein;
		} else {
			//�ҵ����еĺ���������ֵ��С��Node�����ڵ�
			Node<T>* pTempNode = _firstChild;
			while (pTempNode->getNextSibling () != nullptr &&
				pTempNode->getNextSibling ()->getIndex () < nodein->getIndex ())
				pTempNode = pTempNode->getNextSibling ();
			//�ýڵ�����һ���ֵܽڵ�
			if (pTempNode->getNextSibling () == nullptr) {
				pTempNode->setNextSibling (nodein);
				nodein->setParenet (this);
			} else {  //�ýڵ��������ڵ�֮��
				nodein->setParenet (this);
				nodein->setNextSibling (pTempNode->getNextSibling ());
				pTempNode->setNextSibling (nodein);
			}
		}
	}

	Node<T>* BNodeSearch (int index) {
		Node<T>* tempNode = nullptr;
		if (_index == index) {
			return this;
		}
		if (_left != nullptr) {
			tempNode = _left->BNodeSearch (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}

		if (_right != nullptr) {
			tempNode = _right->BNodeSearch (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}
		return nullptr;
	}

	Node<T>* NodeSearch (int index) {
		Node<T>* tempNode = nullptr;

		if (_index == index) {
			return this;
		}
		if (_firstChild != nullptr) {
			tempNode = _firstChild->NodeSearch (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}

		if (_sibling != nullptr) {
			tempNode = _sibling->NodeSearch (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}

		return nullptr;
	}

	/*��ѯ������Ҷ����*/

	int NodeLeavesCount (int leaves) {
		if (this->_left != nullptr)
			leaves = this->_left->NodeLeavesCount (leaves);

		if (this->_right != nullptr)
			leaves = this->_right->NodeLeavesCount (leaves);

		if (this->getLChild () == nullptr && this->getRChild () == nullptr)
			leaves++;

		return leaves;
	}

	/*��ѯ���������Ľڵ���(�����Լ�)*/

	int BiNodeChildrenCount () {
		int biCnt = 0;

		if (this->_left != nullptr)
			biCnt += this->_left->BiNodeChildrenCount ();

		if (this->_right != nullptr)
			biCnt += this->_right->BiNodeChildrenCount ();

		biCnt++;
		return biCnt;
	}

	/*��ѯ�����Ľڵ���(�����Լ�)*/

	int NodeChildrenCount () {
		int tiCnt = 0;

		if (this->_firstChild != nullptr)
			tiCnt += this->_firstChild->NodeChildrenCount ();

		if (this->_sibling != nullptr)
			tiCnt += this->_sibling->NodeChildrenCount ();

		tiCnt++;
		return tiCnt;
	}

	/*ɾ���������������нڵ�*/

	int BTreeDeleteAll () {
		int Times = 0;
		if (this->_left != nullptr) {
			Times += this->_left->BTreeDeleteAll ();
			this->_left = nullptr;
		}

		if (this->_right != nullptr) {
			Times += this->_right->BTreeDeleteAll ();
			this->_right = nullptr;
		}

		Times++;
		delete this;
		return Times;
	}

	/*ɾ�����������нڵ�*/

	int NodeDelete () {
		int Times = 0;
		if (this->_firstChild != nullptr) {
			Times += this->_firstChild->NodeDelete ();
			this->_firstChild = nullptr;
		}

		if (this->_sibling != nullptr) {
			Times += this->_sibling->NodeDelete ();
			this->_sibling = nullptr;
		}

		Times++;
		delete this;
		return Times;
	}

	/*-----------------------traversal------------------------*/

	int BiNodePreorderTraversal () {
		//cout<<"Index:"<<this->getIndex()<<";Data:"<<this->getData()<<endl;
		int res = this->getIndex ();

		if (this->getLChild () != nullptr)
			res ^= this->getLChild ()->BiNodePreorderTraversal ();

		if (this->getRChild () != nullptr)
			res ^= this->getRChild ()->BiNodePreorderTraversal ();

		return res;
	}

	int NodePreorderTraversal () {
		//cout<<"Index:"<<this->getIndex()<<";Data:"<<this->getData()<<endl;

		int res = this->getIndex ();
		if (this->getFirstChild () != nullptr)
			res ^= this->getFirstChild ()->NodePreorderTraversal ();

		if (this->getNextSibling () != nullptr)
			res ^= this->getNextSibling ()->NodePreorderTraversal ();

		return res;
	}
	///////////////////////////////////////////////////////////////////
	//������ת��Ϊɭ��
	//�������ڵ�����ӽڵ���ҽڵ�����ת��Ϊ�ӽڵ�����
	void _convertToForest () {
		Node<T>* pLeftNode = this->getLChild ();
		if (pLeftNode == nullptr)return;

		Node<T>* pPNode = pLeftNode;
		Node<T>* pRNode;
		while (pPNode != nullptr) {
			pRNode = pPNode->getRChild ();
			pPNode->setRChild (nullptr);//��һ�������ӽڵ�ת��Ϊ��һ���ӽڵ�

			this->setChild (pPNode);
			pPNode->_convertToForest ();

			pPNode = pRNode;
		}
	}

	//ɭ��ת��Ϊ������
	//��һ���ӽڵ������ӽڵ㣬�ֵܽڵ����ҽڵ�����
	void _convertToBTree () {
		Node<T>* pFirstChild = this->getFirstChild ();
		Node<T>* pNextSibling = this->getNextSibling ();

		this->setLChild (pFirstChild);
		this->setRChild (pNextSibling);
		this->setFirstChild (nullptr);
		this->setNextSibling (nullptr);

		if (pFirstChild != nullptr) pFirstChild->_convertToBTree ();
		if (pNextSibling != nullptr) pNextSibling->_convertToBTree ();
	}
	///////////////////////////////////////////////////////////////////


//private:
	int _index;	 //����
	T _data;	 //ֵ

	Node* _parent;		 //���׽ڵ�
	
	//������
	Node* _left;	 //���ӽڵ�
	Node* _right;	 //���ӽڵ�

	//ɭ��
	Node* _firstChild;	 //�ӽڵ�����ͷ��
	Node* _sibling;	 //�ֵܽڵ�
};

template <typename T>
class Tree {
public:

	enum state { FOREST, BTREE };
	Tree (int size, int index, T data) : _root (new Node<T> (index, data)), _size (1), _maxSize (size) {}
	Tree (int size) : _root (new Node<T> (0, 0)), _size (1), _maxSize (size) {}
	~Tree () {
		if (_root != nullptr) delete _root;
		_root = nullptr;
	}

	bool IsTreeEmpty ();	 //���Ƿ�Ϊ��
	bool IsTreeFull ();	 //���������Ƿ�����

	//����
	Node<T>* getBiNodeByIndex (int index);  //ͨ�����������ڵ�
	Node<T>* getNodeByIndex (int index);
	
	//������Ϣ
	int getLeaves ();	 //��ȡ����Ҷ����
	int getHeight ();	 //��ȡ���ĸ߶�(�������ڵ�)
	int getWidth ();		 //��ȡ���Ŀ��(�������ڵ�)
	int getNowBiSize ();	 //��ȡ�����ڵĽڵ������������ڵ㣩
	int getNowTreeSize ();
	int getMaxSize ();  //��ȡ�������ڵ���

	bool addLeftSubTreeByIndex (Node<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		_state = BTREE;
		Node<T>* tempNode;
		tempNode = _root->BNodeSearch (searchIndex);  //ͨ�������ҵ��ýڵ�

		tempNode->setLChild (pNode);
		_size += pNode->BiNodeChildrenCount ();
		return true;
	}

	bool addRightSubTreeByIndex (Node<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		Node<T>* tempNode;
		tempNode = _root->BNodeSearch (searchIndex);  //ͨ�������ҵ��ýڵ�

		tempNode->setRChild (pNode);

		return true;
	}

	bool addSubTreeByIndex (Node<T>* pNode, int searchIndex) {
		if (_root == nullptr)
			return false;
		_state = FOREST;

		Node<T>* tempNode;
		tempNode = _root->NodeSearch (searchIndex);	//ͨ�������ҵ��ýڵ�
		tempNode->setChild (pNode);
		return true;
	}

	bool addChildNodeByIndex (int newIndex, T data, int searchIndex) {
		if (_root == nullptr)
			return false;

		Node<T>* tempNode;
		tempNode = _root->NodeSearch (searchIndex);	//ͨ�������ҵ��ýڵ�

		if (tempNode != nullptr) {
			//cout << tempNode->getIndex() << endl;
			return addChildNodeByNode (newIndex, data, tempNode);
		}

		return false;
	}

	bool addChildNodeByNode (int index, T data, Node<T>* pNode) {
		Node<T>* pNodeCopy = pNode;	 //��pNode�ĸ�������ֹpNode�ı������޸�

		if (IsTreeFull ()) return false;
		//�õ����ӽڵ�
		Node<T>* pFirstChildNode = pNodeCopy->getFirstChild ();
		Node<T>* newNode = new Node<T> (index, data);
		if (pFirstChildNode == nullptr) {
			pNodeCopy->setFirstChild (newNode);
			newNode->setParenet (pNodeCopy);
		} else if (index < pFirstChildNode->getIndex ()) {
			pNodeCopy->setFirstChild (newNode);
			newNode->setParenet (pNodeCopy);
			newNode->setNextSibling (pFirstChildNode);
		} else {
			//�ҵ�����ֵ��С��index�����ڵ�
			Node<T>* pTempNode = pFirstChildNode;
			while (pTempNode->getNextSibling () != nullptr &&
				pTempNode->getNextSibling ()->getIndex () < index)
				pTempNode = pTempNode->getNextSibling ();
			//�ýڵ�����һ���ֵܽڵ�
			if (pTempNode->getNextSibling () == nullptr) {
				pTempNode->setNextSibling (newNode);
				newNode->setParenet (pNodeCopy);
			} else {
				newNode->setParenet (pNodeCopy);
				newNode->setNextSibling (pTempNode->getNextSibling ());
				pTempNode->setNextSibling (newNode);
			}
		}
		_size++;
		return true;
	}

	void addEdge (int a, int b) {
		Node<T>* nodeA, * nodeB, * tempNode;

		nodeA = _root->NodeSearch (a);
		nodeB = _root->NodeSearch (b);
		if (a < b) {
			tempNode = nodeA;
			//�ҵ�nodeB��ǰһ���ֵܽڵ�
			while (tempNode->getNextSibling () != nodeB)
				tempNode = tempNode->getNextSibling ();

			tempNode->setNextSibling (nodeB->getNextSibling ());
			nodeB->setNextSibling (nullptr);
			nodeB->setParenet (nodeA);
			nodeA->setChild (nodeB);
		} else if (a > b) {
			//b�ǳ���,��b����һ���ֵܽڵ���Ϊ����
			if (_root->getFirstChild () == nodeB) {
				Node<T>* NextSiblingNode = nodeB->getNextSibling ();
				_root->setFirstChild (NextSiblingNode);
				nodeB->setNextSibling (nullptr);
				nodeA->setChild (nodeB);
			} else {
				tempNode = _root->getFirstChild ();
				while (tempNode->getNextSibling () != nodeB)
					tempNode = tempNode->getNextSibling ();
				tempNode->setNextSibling (nodeB->getNextSibling ());
				nodeB->setNextSibling (nullptr);
				nodeB->setParenet (nodeA);
				nodeA->setChild (nodeB);
			}
		}
	}

	//ɭ��ɾ���ڵ�
	bool forestDeleteNodeByIndex (int index) {
		Node<T>* deleteNode = _root->NodeSearch (index);

		if (deleteNode != nullptr) {
			if (deleteNode == _root) {
				cout << "forestDeleteNodeByIndex():" << index << "�Ǹ��ڵ㲻��ɾ��" << endl;
				return false;
			}
			return deleteNodeIn (deleteNode);
		}
		return false;
	}

	bool deleteNodeIn (Node<T>* pNode) {
		if (pNode != nullptr) {
			//���ӽڵ�
			Node<T>* first_child = pNode->getFirstChild ();
			//�����ӽڵ㵱�����ڵ�ĺ���
			if (first_child != nullptr) {
				Node<T>* p = first_child;
				Node<T>* pp;
				while (p != nullptr) {
					pp = p->getNextSibling ();
					_root->setChild (p);
					p = pp;
				}
			}

			pNode->setFirstChild (nullptr);

			//�ֵ�  �ڸ��ڵ���ӽڵ�������
			Node<T>* pParentNode = pNode->getParent ();
			Node<T>* pCNode = pParentNode->getFirstChild ();
			if (pCNode == pNode) {
				pParentNode->setFirstChild (pNode->getNextSibling ());
			} else {
				while (pCNode->getNextSibling () != pNode) {
					pCNode = pCNode->getNextSibling ();
				}
				pCNode->setNextSibling (pNode->getNextSibling ());
			}

			pNode->setNextSibling (nullptr);
			pNode->setFirstChild (nullptr);
			return true;
		}

		return false;
	}

	void BiPreorderTraversal () {
		if (_root == nullptr) return;
		cout << _root->getLChild ()->BiNodePreorderTraversal () << endl;
	}

	void TreePreorderTraversal () {
		if (_root == nullptr) return;

		Node<T>* pFirstNode = _root->getFirstChild ();
		Node<T>* pSiblingNode;

		int res = pFirstNode->getIndex ();
		//cout << res << endl;
		if (pFirstNode->getFirstChild () != nullptr)
			res ^= pFirstNode->getFirstChild ()->NodePreorderTraversal ();

		pSiblingNode = pFirstNode->getNextSibling ();
		cout << res << " ";

		while (pSiblingNode != nullptr) {
			res = pSiblingNode->getIndex ();
			if (pSiblingNode->getFirstChild () != nullptr)
				res ^= pSiblingNode->getFirstChild ()->NodePreorderTraversal ();
			if (pSiblingNode->getNextSibling () != nullptr) {
				cout << res << " ";
			} else {
				cout << res;
			}
			pSiblingNode = pSiblingNode->getNextSibling ();
		}
		cout << endl;
	}

	void toForest () { 
		_state = FOREST;
		_root->_convertToForest ();
	}
	void toBTree () { 
		_state = BTREE;
		_root->_convertToBTree ();
	}
	enum state getState () { return _state; }


	ostream& show (ostream& out) {
		if (_state == FOREST) {
			//showForest (out);
			printForest ();
		} else if(_state == BTREE) {
			//showBtree (out);
			printBTree ();
		}

		return out;
	}


//////////////////��ӡ
	void printForest () {
		for (int i = 0; i < 10; i++) {

			treedown[i] = 1;
		}
		showTree (_root, 0);
	}
	void printForestin (Node<T>* rootin, int& row, int deepthin) {
		

	}
	bool treedown[10];
	void showTree (Node<T>* rootin,int deepth) {

		for (int i = 1; i <= deepth; i++) {
			if (treedown[i] == true) {
				cout << "�� ";
			} else {
				cout << "  ";
			}

		}

		if (rootin->getNextSibling () != NULL) {
			cout << "�� ";

		} else {
			cout << "�� ";
			treedown[deepth+1] = 0;
		}
		cout<< rootin->getIndex () /*<< ";Data:" << rootin->getData () */ << endl;
		if (rootin->getFirstChild () != NULL)
			showTree (rootin->getFirstChild (),deepth + 1);

		if (rootin->getNextSibling () != NULL)
			showTree (rootin->getNextSibling (),deepth);

	}
	void printBTree () {
		for (int i = 0; i < 1000; i++) {
			for (int j = 0; j < 1000; j++) {
				charmap[i][j] = ' ';
			}
			charmap[i][1000 - 1] = '\0';

		}
		int row = 0;
		int deepth = 0;
		printBTreein (_root, row, deepth);

		for (int i = 0; i < 15; i++) {
			cout << charmap[i] << endl;
		}
	}
	int printBTreein (Node<T>* rootin,int& row, int deepthin) {
		if (rootin == nullptr) {
			return row++;
		}
		int top= printBTreein(rootin->_left,row, deepthin+1);

		int me = row;
		if (top != me-1) {
			charmap[top][deepthin * 3] = '/';
			charmap[top][deepthin * 3 + 1] = '-';
			charmap[top][deepthin * 3 + 2] = '-';
		}
		int bot= printBTreein (rootin->_right, row, deepthin+1);
		if (bot != me) {
			charmap[bot][deepthin * 3] = '\\';
			charmap[bot][deepthin * 3 + 1] = '-';
			charmap[bot][deepthin * 3 + 2] = '-';
		}

		for (int i = top + 1; i < bot; i++) {
			charmap[i][deepthin*3] = '|';
		}

		charmap[me][deepthin*3] = '+';
		sprintf(&charmap[me][deepthin * 3],"%d",rootin->_index);
		return me;
	}





protected:
	enum state _state;

	char charmap[1000][1000]; //���ڴ��Ҫ��ӡ�ĺ�������� 

	//void toForest () { _root->toForest (); }
	//void toBTree () { _root->toBTree (); }
	Node<T>* _root;	 //�����ڵ�
	int _size;			 //��ǰ���Ľڵ���(���������ڵ�)
	int _maxSize;		 //�������ڵ���(���������ڵ�)
};

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#pragma warning(disable : 4996)
int main () {
	FILE* fd  = freopen ("in2.txt", "r", stdin);

	int i, j, K, M, N, Q, node, rootIndex, pos;
	Tree<int>* tree = new Tree<int> (5302, 0, 0);

	//�ڵ��ʼ��
	Node<int>* nodes = new Node<int>[5302];
	for (i = 0; i < 5102; i++) {
		nodes[i].setIndex (i);
	}

	cin >> K >> M >> N;

	//K=0��ʼ��ɭ��
	if (K == 0) {
		//MΪ�����ڵ�ĸ���
		int* roots = new int[M];
		int A, B;
		for (i = 0; i < M; i++) cin >> roots[i];

		//A�ڵ���B������,������ÿ���ڵ㶼���Լ��ĵ�ַ��ͨ����ַ���нڵ��ϵ�ı���
		for (i = 0; i < N; i++) {
			cin >> A >> B;
			for (j = 0; j < B; j++) {
				cin >> node;
				nodes[A].setChild (&nodes[node]);
			}
		}

		for (i = 0; i < M; i++) {
			tree->addSubTreeByIndex (&nodes[roots[i]], 0);
		}
	} else {//��ʼ��������
		int A, l, r;

		cin >> rootIndex;

		for (i = 0; i < N; i++) {
			cin >> A >> l >> r;
			nodes[A].setIndex (A);
			if (l == -1)
				nodes[A].setLChild (nullptr);
			else
				nodes[A].setLChild (&nodes[l]);

			if (r == -1)
				nodes[A].setRChild (nullptr);
			else
				nodes[A].setRChild (&nodes[r]);

		}
		tree->addLeftSubTreeByIndex (&nodes[rootIndex], 0);
		Sleep (500);
		system ("cls");
		tree->show (cout);
	}
	freopen ("CON", "r", stdin);

	//��ʾͼ�ν��棬�����û�����
	int father, a, b;
	while(1){
		system ("cls");
				cout<< R"( ______                  _     ____ _______            
|  ____|                | |   |  _ \__   __|           
| |__ ___  _ __ ___  ___| |_  | |_) | | |_ __ ___  ___ 
|  __/ _ \| '__/ _ \/ __| __| |  _ <  | | '__/ _ \/ _ \
| | | (_) | | |  __/\__ \ |_  | |_) | | | | |  __/  __/
|_|  \___/|_|  \___||___/\__| |____/  |_|_|  \___|\___|
)";
		tree->show (cout);
		if (tree->getState () == Tree<int>::BTREE) {
			cout << "*�����Ƕ�����ģʽ\n";
		}else {
			cout << "*������ɭ��ģʽ\n";
		}
		cout << R"(*������ָ�
*  1 father node ��ӽڵ�
*  2 father node ɾ���ڵ�
*  3 a b ��ӱ�
*  4 ����ɭ��ת��
*  5 pos(0��1��) father node ����������ӽڵ�)";
		cout << endl;
		int op;
		cin >> op;
		if (op == 1) {
			cin >> father >> node;
			if (father == -1) {
				tree->addSubTreeByIndex (&nodes[node], 0);
			} else {
				tree->addSubTreeByIndex (&nodes[node], father);
			}
		} else if (op == 2) {
			cin >> father >> node;
			tree->forestDeleteNodeByIndex (node);
		} else if (op == 3) {
			cin >> a >> b;
			tree->addEdge (a, b);
		} else if (op == 4) {
			if (K == 0) {
				tree->toBTree ();
				K = 1;
			} else {
				tree->toForest ();
				K = 0;
			}
		} else if (op == 5) {
			cin >> pos >> father >> node;
			if (pos == 0)
				tree->addRightSubTreeByIndex (&nodes[node], father);
			else
				tree->addLeftSubTreeByIndex (&nodes[node], father);
		} else {
			if (K == 0)
				tree->TreePreorderTraversal ();
			else
				tree->BiPreorderTraversal ();
		}
	}
	return 0;
}
