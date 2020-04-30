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
			_left->bTreeDeleteAll ();
			_left = nullptr;
		}

		if (_right != nullptr) {
			_right->bTreeDeleteAll ();
			_right = nullptr;
		}

		if (_firstChild != nullptr) {
			_firstChild->forestDelete ();
			_firstChild = nullptr;
		}

		if (_sibling != nullptr) {
			_sibling->forestDelete ();
			_sibling = nullptr;
		}
		_parent = nullptr;
	}

	//get data
	int getIndex () { return _index; }
	T getData () { return _data; }
	Node<T>* getParent () { return _parent; }
	Node<T>* bTreeGetLChild () { return _left; }
	Node<T>* bTreeGetRChild () { return _right; }
	Node<T>* forestGetFirstChild () { return _firstChild; }
	Node<T>* forestGetNextSibling () { return _sibling; }

	void setIndex (int index) { _index = index; }
	void setData (T data) { _data = data; }
	void setParenet (Node* Node) { _parent = Node; }
	void bTreeSetLChild (Node* Node) {
		_left = Node;
		if (Node != nullptr) Node->setParenet (this);
	}
	void bTreesetRChild (Node* Node) {
		_right = Node;
		if (Node != nullptr) Node->setParenet (this);
	}
	void forestSetFirstChild (Node* Node) { _firstChild = Node; }
	void forestSetNextSibling (Node* Node) { _sibling = Node; }
	void forestSetChild (Node* nodein) {	 //�޺��ӵ����
		if (_firstChild == nullptr) {
			_firstChild = nodein;
			nodein->setParenet (this);
		}

		//��һ�����ӵ�����ֵ����Node������ֵ�����
		else if (nodein->getIndex () < _firstChild->getIndex ()) {
			nodein->setParenet (this);
			nodein->forestSetNextSibling (_firstChild);
			_firstChild = nodein;
		} else {//���򣡣�������
			//�ҵ����еĺ���������ֵ��С��Node�����ڵ�
			Node<T>* pTempNode = _firstChild;
			while (pTempNode->forestGetNextSibling () != nullptr &&
				pTempNode->forestGetNextSibling ()->getIndex () < nodein->getIndex ())
				pTempNode = pTempNode->forestGetNextSibling ();
			//�ýڵ�����һ���ֵܽڵ�
			if (pTempNode->forestGetNextSibling () == nullptr) {
				pTempNode->forestSetNextSibling (nodein);
				nodein->setParenet (this);
			} else {  //�ýڵ��������ڵ�֮��
				nodein->setParenet (this);
				nodein->forestSetNextSibling (pTempNode->forestGetNextSibling ());
				pTempNode->forestSetNextSibling (nodein);
			}
		}
	}

	Node<T>* bTreeSearchByIndex (int index) {
		Node<T>* tempNode = nullptr;
		if (_index == index) {
			return this;
		}
		if (_left != nullptr) {
			tempNode = _left->bTreeSearchByIndex (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}

		if (_right != nullptr) {
			tempNode = _right->bTreeSearchByIndex (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}
		return nullptr;
	}

		//�ݹ����Ŀ�������ͺ�����
	Node<T>* forestSearchNodeByIndex (int index) {
		Node<T>* tempNode = nullptr;
		if (_index == index) {
			return this;
		}
		if (_firstChild != nullptr) {
			tempNode = _firstChild->forestSearchNodeByIndex (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}

		if (_sibling != nullptr) {
			tempNode = _sibling->forestSearchNodeByIndex (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}

		return nullptr;
	}

	/*��ѯ������Ҷ����*/
	int bTreeLeavesCount (int leaves) {
		if (this->_left != nullptr)
			leaves = this->_left->bTreeLeavesCount (leaves);

		if (this->_right != nullptr)
			leaves = this->_right->bTreeLeavesCount (leaves);

		if (this->bTreeGetLChild () == nullptr && this->bTreeGetRChild () == nullptr)
			leaves++;

		return leaves;
	}

	/*��ѯ���������Ľڵ���(�����Լ�)*/
	int bTreeCountChildren () {
		int biCnt = 0;

		if (this->_left != nullptr)
			biCnt += this->_left->bTreeCountChildren ();

		if (this->_right != nullptr)
			biCnt += this->_right->bTreeCountChildren ();

		biCnt++;
		return biCnt;
	}

	/*��ѯ�����Ľڵ���(�����Լ�)*/

	int forestChildrenCount () {
		int tiCnt = 0;

		if (this->_firstChild != nullptr)
			tiCnt += this->_firstChild->forestChildrenCount ();

		if (this->_sibling != nullptr)
			tiCnt += this->_sibling->forestChildrenCount ();

		tiCnt++;
		return tiCnt;
	}

	/*ɾ���������������нڵ�*/

	int bTreeDeleteAll () {
		int Times = 0;
		if (this->_left != nullptr) {
			Times += this->_left->bTreeDeleteAll ();
			this->_left = nullptr;
		}

		if (this->_right != nullptr) {
			Times += this->_right->bTreeDeleteAll ();
			this->_right = nullptr;
		}

		Times++;
		delete this;
		return Times;
	}

	/*ɾ�����������нڵ�*/

	int forestDelete () {
		int Times = 0;
		if (this->_firstChild != nullptr) {
			Times += this->_firstChild->forestDelete ();
			this->_firstChild = nullptr;
		}

		if (this->_sibling != nullptr) {
			Times += this->_sibling->forestDelete ();
			this->_sibling = nullptr;
		}

		Times++;
		delete this;
		return Times;
	}

	int bTreePreorderTraversal () {
		int res = this->getIndex ();

		if (this->bTreeGetLChild () != nullptr)
			res ^= this->bTreeGetLChild ()->bTreePreorderTraversal ();

		if (this->bTreeGetRChild () != nullptr)
			res ^= this->bTreeGetRChild ()->bTreePreorderTraversal ();

		return res;
	}

	int NodePreorderTraversal () {
		//cout<<"Index:"<<this->getIndex()<<";Data:"<<this->getData()<<endl;

		int res = this->getIndex ();
		if (this->forestGetFirstChild () != nullptr)
			res ^= this->forestGetFirstChild ()->NodePreorderTraversal ();

		if (this->forestGetNextSibling () != nullptr)
			res ^= this->forestGetNextSibling ()->NodePreorderTraversal ();

		return res;
	}
	///////////////////////////////////////////////////////////////////
	//������ת��Ϊɭ��
	//�������ڵ�����ӽڵ���ҽڵ�����ת��Ϊ�ӽڵ�����
	void _convertToForest () {
		Node<T>* pLeftNode = this->bTreeGetLChild ();
		if (pLeftNode == nullptr)return;

		Node<T>* pPNode = pLeftNode;
		Node<T>* pRNode;
		while (pPNode != nullptr) {
			pRNode = pPNode->bTreeGetRChild ();
			pPNode->bTreesetRChild (nullptr);//��һ�������ӽڵ�ת��Ϊ��һ���ӽڵ�

			this->forestSetChild (pPNode);
			pPNode->_convertToForest ();

			pPNode = pRNode;
		}
	}

	//ɭ��ת��Ϊ������
	//��һ���ӽڵ������ӽڵ㣬�ֵܽڵ����ҽڵ�����
	void _convertToBTree () {
		Node<T>* pFirstChild = this->forestGetFirstChild ();
		Node<T>* pNextSibling = this->forestGetNextSibling ();

		this->bTreeSetLChild (pFirstChild);
		this->bTreesetRChild (pNextSibling);
		this->forestSetFirstChild (nullptr);
		this->forestSetNextSibling (nullptr);

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
	Node<T>* getbNodeByIndex (int index);  //ͨ�����������ڵ�
	Node<T>* getNodeByIndex (int index);
	
	//������Ϣ
	int getLeaves ();	 //��ȡ����Ҷ����
	int getHeight ();	 //��ȡ���ĸ߶�(�������ڵ�)
	int getWidth ();		 //��ȡ���Ŀ��(�������ڵ�)
	int getNowbSize ();	 //��ȡ�����ڵĽڵ������������ڵ㣩
	int getNowTreeSize ();
	int getMaxSize ();  //��ȡ�������ڵ���

	bool addLeftSubTreeByIndex (Node<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		_state = BTREE;
		Node<T>* tempNode;
		tempNode = _root->bTreeSearchByIndex (searchIndex);  //ͨ�������ҵ��ýڵ�

		tempNode->bTreeSetLChild (pNode);
		_size += pNode->bTreeCountChildren ();
		return true;
	}

	bool addRightSubTreeByIndex (Node<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		Node<T>* tempNode;
		tempNode = _root->bTreeSearchByIndex (searchIndex);  //ͨ�������ҵ��ýڵ�

		tempNode->bTreesetRChild (pNode);

		return true;
	}

	bool forestAddSubTreeByIndex (Node<T>* new_node, int indexi) {
		if (_root == nullptr)return false;
		_state = FOREST;

		Node<T>* tempNode;
		tempNode = _root->forestSearchNodeByIndex (indexi);	//ͨ�������ҵ��ýڵ�
		tempNode->forestSetChild (new_node);
		return true;
	}

	bool addChildNodeByIndex (int newIndex, T data, int searchIndex) {
		if (_root == nullptr)
			return false;

		Node<T>* tempNode;
		tempNode = _root->forestSearchNodeByIndex (searchIndex);	//ͨ�������ҵ��ýڵ�

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
		Node<T>* pFirstChildNode = pNodeCopy->forestGetFirstChild ();
		Node<T>* newNode = new Node<T> (index, data);
		if (pFirstChildNode == nullptr) {
			pNodeCopy->forestSetFirstChild (newNode);
			newNode->setParenet (pNodeCopy);
		} else if (index < pFirstChildNode->getIndex ()) {
			pNodeCopy->forestSetFirstChild (newNode);
			newNode->setParenet (pNodeCopy);
			newNode->forestSetNextSibling (pFirstChildNode);
		} else {
			//�ҵ�����ֵ��С��index�����ڵ�
			Node<T>* pTempNode = pFirstChildNode;
			while (pTempNode->forestGetNextSibling () != nullptr &&
				pTempNode->forestGetNextSibling ()->getIndex () < index)
				pTempNode = pTempNode->forestGetNextSibling ();
			//�ýڵ�����һ���ֵܽڵ�
			if (pTempNode->forestGetNextSibling () == nullptr) {
				pTempNode->forestSetNextSibling (newNode);
				newNode->setParenet (pNodeCopy);
			} else {
				newNode->setParenet (pNodeCopy);
				newNode->forestSetNextSibling (pTempNode->forestGetNextSibling ());
				pTempNode->forestSetNextSibling (newNode);
			}
		}
		_size++;
		return true;
	}

	void addEdge (int a, int b) {
		Node<T>* nodeA, * nodeB, * tempNode;

		nodeA = _root->forestSearchNodeByIndex (a);
		nodeB = _root->forestSearchNodeByIndex (b);
		if (a < b) {
			tempNode = nodeA;
			//�ҵ�nodeB��ǰһ���ֵܽڵ�
			while (tempNode->forestGetNextSibling () != nodeB)
				tempNode = tempNode->forestGetNextSibling ();

			tempNode->forestSetNextSibling (nodeB->forestGetNextSibling ());
			nodeB->forestSetNextSibling (nullptr);
			nodeB->setParenet (nodeA);
			nodeA->forestSetChild (nodeB);
		} else if (a > b) {
			//b�ǳ���,��b����һ���ֵܽڵ���Ϊ����
			if (_root->forestGetFirstChild () == nodeB) {
				Node<T>* NextSiblingNode = nodeB->forestGetNextSibling ();
				_root->forestSetFirstChild (NextSiblingNode);
				nodeB->forestSetNextSibling (nullptr);
				nodeA->forestSetChild (nodeB);
			} else {
				tempNode = _root->forestGetFirstChild ();
				while (tempNode->forestGetNextSibling () != nodeB)
					tempNode = tempNode->forestGetNextSibling ();
				tempNode->forestSetNextSibling (nodeB->forestGetNextSibling ());
				nodeB->forestSetNextSibling (nullptr);
				nodeB->setParenet (nodeA);
				nodeA->forestSetChild (nodeB);
			}
		}
	}

	//ɭ��ɾ���ڵ�
	bool forestDeleteNodeByIndex (int index) {
		Node<T>* deleteNode = _root->forestSearchNodeByIndex (index);

		if (deleteNode != nullptr) {
			if (deleteNode == _root) {
				cout << "forestDeleteNodeByIndex():" << index << "�Ǹ��ڵ㲻��ɾ��" << endl;
				return false;
			}
			return forestDeleteNodeByP (deleteNode);
		}
		return false;
	}

	bool forestDeleteNodeByP (Node<T>* pNode) {
		if (pNode != nullptr) {
			//���ӽڵ�
			Node<T>* first_child = pNode->forestGetFirstChild ();
			//�����ӽڵ㵱�����ڵ�ĺ���
			if (first_child != nullptr) {
				Node<T>* p = first_child;
				Node<T>* pp;
				while (p != nullptr) {
					pp = p->forestGetNextSibling ();
					_root->forestSetChild (p);
					p = pp;
				}
			}

			pNode->forestSetFirstChild (nullptr);

			//�ֵ�  �ڸ��ڵ���ӽڵ�������
			Node<T>* pParentNode = pNode->getParent ();
			Node<T>* pCNode = pParentNode->forestGetFirstChild ();
			if (pCNode == pNode) {
				pParentNode->forestSetFirstChild (pNode->forestGetNextSibling ());
			} else {
				while (pCNode->forestGetNextSibling () != pNode) {
					pCNode = pCNode->forestGetNextSibling ();
				}
				pCNode->forestSetNextSibling (pNode->forestGetNextSibling ());
			}

			pNode->forestSetNextSibling (nullptr);
			pNode->forestSetFirstChild (nullptr);
			return true;
		}

		return false;
	}

	void bPreorderTraversal () {
		if (_root == nullptr) return;
		cout << _root->bTreeGetLChild ()->bTreePreorderTraversal () << endl;
	}

	void TreePreorderTraversal () {
		if (_root == nullptr) return;

		Node<T>* pFirstNode = _root->forestGetFirstChild ();
		Node<T>* pSiblingNode;

		int res = pFirstNode->getIndex ();
		//cout << res << endl;
		if (pFirstNode->forestGetFirstChild () != nullptr)
			res ^= pFirstNode->forestGetFirstChild ()->NodePreorderTraversal ();

		pSiblingNode = pFirstNode->forestGetNextSibling ();
		cout << res << " ";

		while (pSiblingNode != nullptr) {
			res = pSiblingNode->getIndex ();
			if (pSiblingNode->forestGetFirstChild () != nullptr)
				res ^= pSiblingNode->forestGetFirstChild ()->NodePreorderTraversal ();
			if (pSiblingNode->forestGetNextSibling () != nullptr) {
				cout << res << " ";
			} else {
				cout << res;
			}
			pSiblingNode = pSiblingNode->forestGetNextSibling ();
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

		if (rootin->forestGetNextSibling () != NULL) {
			cout << "�� ";

		} else {
			cout << "�� ";
			treedown[deepth+1] = 0;
		}
		cout<< rootin->getIndex () /*<< ";Data:" << rootin->getData () */ << endl;
		if (rootin->forestGetFirstChild () != NULL)
			showTree (rootin->forestGetFirstChild (),deepth + 1);

		if (rootin->forestGetNextSibling () != NULL)
			showTree (rootin->forestGetNextSibling (),deepth);

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
	Tree<int>* tree = new Tree<int> (6000, 0, 0);

	//�ڵ��ʼ��
	Node<int>* nodes = new Node<int>[6000];
	for (i = 0; i < 6000; i++) {
		nodes[i].setIndex (i);
	}

	cin >> K >> M >> N;

	//K=0��ʼ��ɭ��
	if (K == 0) {
		//MΪ�����ڵ�ĸ���
		int* roots = new int[M];
		int A, B;

		//��ȡ���ڵ�
		for (i = 0; i < M; i++)
			cin >> roots[i];

		//A�ڵ���B������,������ÿ���ڵ㶼���Լ��ĵ�ַ��ͨ����ַ���нڵ��ϵ�ı���
		for (i = 0; i < N; i++) {
			cin >> A >> B;
			for (j = 0; j < B; j++) {//���к��ӽڵ�ı��
				cin >> node;
				nodes[A].forestSetChild (&nodes[node]);
			}
		}
		//���ڵ���ӵ�α���ڵ�
		for (i = 0; i < M; i++) {
			tree->forestAddSubTreeByIndex (&nodes[roots[i]], 0);
		}
	} else {//��ʼ��������
		int A, l, r;

		cin >> rootIndex;

		for (i = 0; i < N; i++) {
			cin >> A >> l >> r;
			nodes[A].setIndex (A);
			if (l == -1)
				nodes[A].bTreeSetLChild (nullptr);
			else
				nodes[A].bTreeSetLChild (&nodes[l]);

			if (r == -1)
				nodes[A].bTreesetRChild (nullptr);
			else
				nodes[A].bTreesetRChild (&nodes[r]);

		}
		tree->addLeftSubTreeByIndex (&nodes[rootIndex], 0);
	}
	freopen ("CON", "r", stdin);

	tree->toForest ();
	K = 0;

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
		if (op == 1) {//����
			cin >> father >> node;
			if (father == -1) {
				tree->forestAddSubTreeByIndex (&nodes[node], 0);
			} else {
				tree->forestAddSubTreeByIndex (&nodes[node], father);
			}
		} else if (op == 2) {//ɾ��
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
				tree->bPreorderTraversal ();
		}
	}
	return 0;
}
