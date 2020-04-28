#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template <typename T>
class TreeNode {
public:

	TreeNode () : _index (0), _parent (0), _leftChild (0), _rightChild (0), _firstChild (0), _nextSibling (0), _data (0) {}
	TreeNode (int index, T data) : _index (index), _data (data), _parent (0), _leftChild (0), _rightChild (0), _firstChild (0), _nextSibling (0) {}
	~TreeNode () {
		if (_leftChild != nullptr) {
			_leftChild->BTreeDeleteAll ();
			_leftChild = nullptr;
		}

		if (_rightChild != nullptr) {
			_rightChild->BTreeDeleteAll ();
			_rightChild = nullptr;
		}

		if (_firstChild != nullptr) {
			_firstChild->TreeNodeDelete ();
			_firstChild = nullptr;
		}

		if (_nextSibling != nullptr) {
			_nextSibling->TreeNodeDelete ();
			_nextSibling = nullptr;
		}
		_parent = nullptr;
	}

	//get data
	int getIndex () { return _index; }
	T getData () { return _data; }
	TreeNode<T>* getParent () { return _parent; }
	TreeNode<T>* getLChild () { return _leftChild; }
	TreeNode<T>* getRChild () { return _rightChild; }
	TreeNode<T>* getFirstChild () { return _firstChild; }
	TreeNode<T>* getNextSibling () { return _nextSibling; }

	void setIndex (int index) { _index = index; }
	void setData (T data) { _data = data; }
	void setParenet (TreeNode* Node) { _parent = Node; }
	void setLChild (TreeNode* Node) {
		_leftChild = Node;
		if (Node != nullptr) Node->setParenet (this);
	}
	void setRChild (TreeNode* Node) {
		_rightChild = Node;
		if (Node != nullptr) Node->setParenet (this);
	}
	void setFirstChild (TreeNode* Node) { _firstChild = Node; }
	void setNextSibling (TreeNode* Node) { _nextSibling = Node; }
	void setChild (TreeNode* Node) {	 //�޺��ӵ����
		if (_firstChild == nullptr) {
			_firstChild = Node;
			Node->setParenet (this);
		}
		//��һ�����ӵ�����ֵ����Node������ֵ�����
		else if (Node->getIndex () < _firstChild->getIndex ()) {
			Node->setParenet (this);
			Node->setNextSibling (_firstChild);
			_firstChild = Node;
		} else {
			//�ҵ����еĺ���������ֵ��С��Node�����ڵ�
			TreeNode<T>* pTempNode = _firstChild;
			while (pTempNode->getNextSibling () != nullptr &&
				pTempNode->getNextSibling ()->getIndex () < Node->getIndex ())
				pTempNode = pTempNode->getNextSibling ();
			//�ýڵ�����һ���ֵܽڵ�
			if (pTempNode->getNextSibling () == nullptr) {
				pTempNode->setNextSibling (Node);
				Node->setParenet (this);
			} else {  //�ýڵ��������ڵ�֮��
				Node->setParenet (this);
				Node->setNextSibling (pTempNode->getNextSibling ());
				pTempNode->setNextSibling (Node);
			}
		}
	}

	/*-----------------------others------------------------*/
	TreeNode<T>* BiNodeSearch (int index) {
		TreeNode<T>* tempNode = nullptr;
		if (_index == index) {
			return this;
		}
		if (_leftChild != nullptr) {
			tempNode = _leftChild->BiNodeSearch (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}

		if (_rightChild != nullptr) {
			tempNode = _rightChild->BiNodeSearch (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}
		return nullptr;
	}

	TreeNode<T>* TreeNodeSearch (int index) {
		TreeNode<T>* tempNode = nullptr;

		if (_index == index) {
			return this;
		}
		if (_firstChild != nullptr) {
			tempNode = _firstChild->TreeNodeSearch (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}

		if (_nextSibling != nullptr) {
			tempNode = _nextSibling->TreeNodeSearch (index);
			if (tempNode != nullptr) {
				return tempNode;
			}
		}

		return nullptr;
	}

	/*��ѯ������Ҷ����*/

	int NodeLeavesCount (int leaves) {
		if (this->_leftChild != nullptr)
			leaves = this->_leftChild->NodeLeavesCount (leaves);

		if (this->_rightChild != nullptr)
			leaves = this->_rightChild->NodeLeavesCount (leaves);

		if (this->getLChild () == nullptr && this->getRChild () == nullptr)
			leaves++;

		return leaves;
	}

	/*��ѯ���������Ľڵ���(�����Լ�)*/

	int BiNodeChildrenCount () {
		int biCnt = 0;

		if (this->_leftChild != nullptr)
			biCnt += this->_leftChild->BiNodeChildrenCount ();

		if (this->_rightChild != nullptr)
			biCnt += this->_rightChild->BiNodeChildrenCount ();

		biCnt++;
		return biCnt;
	}

	/*��ѯ�����Ľڵ���(�����Լ�)*/

	int TreeNodeChildrenCount () {
		int tiCnt = 0;

		if (this->_firstChild != nullptr)
			tiCnt += this->_firstChild->TreeNodeChildrenCount ();

		if (this->_nextSibling != nullptr)
			tiCnt += this->_nextSibling->TreeNodeChildrenCount ();

		tiCnt++;
		return tiCnt;
	}

	/*ɾ���������������нڵ�*/

	int BTreeDeleteAll () {
		int Times = 0;
		if (this->_leftChild != nullptr) {
			Times += this->_leftChild->BTreeDeleteAll ();
			this->_leftChild = nullptr;
		}

		if (this->_rightChild != nullptr) {
			Times += this->_rightChild->BTreeDeleteAll ();
			this->_rightChild = nullptr;
		}

		Times++;
		delete this;
		return Times;
	}

	/*ɾ�����������нڵ�*/

	int TreeNodeDelete () {
		int Times = 0;
		if (this->_firstChild != nullptr) {
			Times += this->_firstChild->TreeNodeDelete ();
			this->_firstChild = nullptr;
		}

		if (this->_nextSibling != nullptr) {
			Times += this->_nextSibling->TreeNodeDelete ();
			this->_nextSibling = nullptr;
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

	int TreeNodePreorderTraversal () {
		//cout<<"Index:"<<this->getIndex()<<";Data:"<<this->getData()<<endl;

		int res = this->getIndex ();
		if (this->getFirstChild () != nullptr)
			res ^= this->getFirstChild ()->TreeNodePreorderTraversal ();

		if (this->getNextSibling () != nullptr)
			res ^= this->getNextSibling ()->TreeNodePreorderTraversal ();

		return res;
	}

	//����0�Ÿ��ڵ�󣬴��������Ӧ����һ���Ĺ���
	void ConvertToForest () {
		TreeNode<T>* pLeftNode = this->getLChild ();

		//this�ڵ���Һ��ӽڵ����û�л����Ѿ�������ˣ��������ӽڵ㣬���ؼ���
		if (pLeftNode == nullptr)
			return;

		//�������ӣ����ӵ��Һ��ӽڵ㣬�Լ����ӵ��Һ��ӵ��Һ��ӽڵ��
		TreeNode<T>* pPNode = pLeftNode;
		TreeNode<T>* pRNode;
		while (pPNode != nullptr) {
			pRNode = pPNode->getRChild ();
			pPNode->setRChild (nullptr);

			this->setChild (pPNode);
			pPNode->ConvertToForest ();

			pPNode = pRNode;
		}
	}

	//Ӧ��Ҳ�ǵݹ�
	void ConvertToBTree () {
		TreeNode<T>* pFirstChild = this->getFirstChild ();
		TreeNode<T>* pNextSibling = this->getNextSibling ();

		//����Ϊ���ӣ���һ���ֵ�Ϊ�Һ���
		this->setLChild (pFirstChild);
		this->setRChild (pNextSibling);
		this->setFirstChild (nullptr);
		this->setNextSibling (nullptr);

		if (pFirstChild != nullptr) pFirstChild->ConvertToBTree ();

		if (pNextSibling != nullptr) pNextSibling->ConvertToBTree ();
	}


//private:
	int _index;	 //����
	T _data;	 //ֵ

	TreeNode* _parent;		 //���׽ڵ�
	TreeNode* _leftChild;	 //���ӽڵ�
	TreeNode* _rightChild;	 //�Һ��ӽڵ�
	TreeNode* _firstChild;	 //���ӽڵ�
	TreeNode* _nextSibling;	 //�ֵܽڵ�
};

template <typename T>
class Tree {
public:
	enum state { FOREST, BTREE };
	Tree (int size, int index, T data) : _root (new TreeNode<T> (index, data)), _size (1), _maxSize (size) {}
	Tree (int size) : _root (new TreeNode<T> (0, 0)), _size (1), _maxSize (size) {}
	~Tree () {
		if (_root != nullptr) delete _root;
		_root = nullptr;
	}

	bool IsTreeEmpty ();	 //���Ƿ�Ϊ��
	bool IsTreeFull ();	 //���������Ƿ�����

	//search
	TreeNode<T>* getBiNodeByIndex (int index);  //ͨ�����������ڵ�
	TreeNode<T>* getTreeNodeByIndex (int index);
	int getLeaves ();	 //��ȡ����Ҷ����
	int getHeight ();	 //��ȡ���ĸ߶�(�������ڵ�)
	int getWidth ();		 //��ȡ���Ŀ��(�������ڵ�)
	int getNowBiSize ();	 //��ȡ�����ڵĽڵ������������ڵ㣩
	int getNowTreeSize ();
	int getMaxSize ();  //��ȡ�������ڵ���

	bool addLeftSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		_state = BTREE;
		TreeNode<T>* tempNode;
		tempNode = _root->BiNodeSearch (searchIndex);  //ͨ�������ҵ��ýڵ�

		tempNode->setLChild (pNode);
		_size += pNode->BiNodeChildrenCount ();
		return true;
	}

	bool addRightSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		TreeNode<T>* tempNode;
		tempNode = _root->BiNodeSearch (searchIndex);  //ͨ�������ҵ��ýڵ�

		tempNode->setRChild (pNode);

		return true;
	}

	bool addSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {
		if (_root == nullptr)
			return false;
		_state = FOREST;

		TreeNode<T>* tempNode;
		tempNode = _root->TreeNodeSearch (searchIndex);	//ͨ�������ҵ��ýڵ�
		tempNode->setChild (pNode);
		return true;
	}

	bool addChildNodeByIndex (int newIndex, T data, int searchIndex) {
		if (_root == nullptr)
			return false;

		TreeNode<T>* tempNode;
		tempNode = _root->TreeNodeSearch (searchIndex);	//ͨ�������ҵ��ýڵ�

		if (tempNode != nullptr) {
			//cout << tempNode->getIndex() << endl;
			return addChildNodeByNode (newIndex, data, tempNode);
		}

		return false;
	}

	bool addChildNodeByNode (int index, T data, TreeNode<T>* pNode) {
		TreeNode<T>* pNodeCopy = pNode;	 //��pNode�ĸ�������ֹpNode�ı������޸�

		if (IsTreeFull ()) return false;
		//�õ����ӽڵ�
		TreeNode<T>* pFirstChildNode = pNodeCopy->getFirstChild ();
		TreeNode<T>* newNode = new TreeNode<T> (index, data);
		if (pFirstChildNode == nullptr) {
			pNodeCopy->setFirstChild (newNode);
			newNode->setParenet (pNodeCopy);
		} else if (index < pFirstChildNode->getIndex ()) {
			pNodeCopy->setFirstChild (newNode);
			newNode->setParenet (pNodeCopy);
			newNode->setNextSibling (pFirstChildNode);
		} else {
			//�ҵ�����ֵ��С��index�����ڵ�
			TreeNode<T>* pTempNode = pFirstChildNode;
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
		TreeNode<T>* nodeA, * nodeB, * tempNode;

		nodeA = _root->TreeNodeSearch (a);
		nodeB = _root->TreeNodeSearch (b);
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
				TreeNode<T>* NextSiblingNode = nodeB->getNextSibling ();
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

	//delete

	bool deleteTreeNodeByIndex (int index) {
		TreeNode<T>* deleteNode = _root->TreeNodeSearch (index);

		if (deleteNode != nullptr) {
			if (deleteNode == _root) {
				cout << "deleteTreeNodeByIndex():" << index << "�Ǹ��ڵ㲻��ɾ��" << endl;
				return false;
			}
			return deleteTreeNodeByNode (deleteNode);
		}
		return false;
	}

	bool deleteTreeNodeByNode (TreeNode<T>* pNode) {
		if (pNode != nullptr) {
			//�ȿ��Ƿ��к��ӽڵ�
			TreeNode<T>* pFirstChildNode = pNode->getFirstChild ();

			//�к��ӽڵ��Ƚ����ӽڵ㶼��Ϊ0�Žڵ���Ӻ���
			if (pFirstChildNode != nullptr) {
				TreeNode<T>* pChildNode = pFirstChildNode;
				TreeNode<T>* pNextSiblingNode;
				while (pChildNode != nullptr) {
					pNextSiblingNode = pChildNode->getNextSibling ();
					_root->setChild (pChildNode);
					pChildNode = pNextSiblingNode;
				}
			}

			pNode->setFirstChild (nullptr);

			//�����꺢�ӽڵ��ٴ����ֵ�����
			TreeNode<T>* pParentNode = pNode->getParent ();
			TreeNode<T>* pCNode = pParentNode->getFirstChild ();
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

		TreeNode<T>* pFirstNode = _root->getFirstChild ();
		TreeNode<T>* pSiblingNode;

		int res = pFirstNode->getIndex ();
		//cout << res << endl;
		if (pFirstNode->getFirstChild () != nullptr)
			res ^= pFirstNode->getFirstChild ()->TreeNodePreorderTraversal ();

		pSiblingNode = pFirstNode->getNextSibling ();
		cout << res << " ";

		while (pSiblingNode != nullptr) {
			res = pSiblingNode->getIndex ();
			if (pSiblingNode->getFirstChild () != nullptr)
				res ^= pSiblingNode->getFirstChild ()->TreeNodePreorderTraversal ();
			if (pSiblingNode->getNextSibling () != nullptr) {
				cout << res << " ";
			} else {
				cout << res;
			}
			pSiblingNode = pSiblingNode->getNextSibling ();
		}
		cout << endl;
	}

	void ConvertToForest () { 
		_state = FOREST;
		_root->ConvertToForest ();
	}
	void ConvertToBTree () { 
		_state = BTREE;
		
		_root->ConvertToBTree ();
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
	void printForestin (TreeNode<T>* rootin, int& row, int deepthin) {
		

	}
	bool treedown[10];
	void showTree (TreeNode<T>* rootin,int deepth) {

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

		for (int i = 0; i < 50; i++) {
			cout << charmap[i] << endl;
		}
	}
	int printBTreein (TreeNode<T>* rootin,int& row, int deepthin) {
		if (rootin == nullptr) {
			return row++;
		}
		int top= printBTreein(rootin->_leftChild,row, deepthin+1);

		int me = row++;
		if (top != me-1) {
			charmap[top][deepthin * 3] = '/';
			charmap[top][deepthin * 3 + 1] = '-';
			charmap[top][deepthin * 3 + 2] = '-';
		}
		int bot= printBTreein (rootin->_rightChild, row, deepthin+1);
		if (bot != me+1) {
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

	//void ConvertToForest () { _root->ConvertToForest (); }
	//void ConvertToBTree () { _root->ConvertToBTree (); }
	TreeNode<T>* _root;	 //�����ڵ�
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
	TreeNode<int>* nodes = new TreeNode<int>[5302];
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

		//A�ڵ���B������
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
	} else {
		//��ʼ��������
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

	//�������Ĳ���
	cin >> Q;
	int op, father, a, b;
	for (i = 0; i < Q; i++) {
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
			tree->deleteTreeNodeByIndex (node);
		} else if (op == 3) {
			cin >> a >> b;
			tree->addEdge (a, b);
		} else if (op == 4) {
			if (K == 0) {
				tree->ConvertToBTree ();
				K = 1;
			} else {

				Sleep (1000);
				system ("cls");
				tree->ConvertToForest ();
				tree->show (cout);
				//Sleep (1000);
				//system ("cls");
				//tree->printForest ();
				K = 0;
			}
		} else if (op == 5) {
			cin >> pos >> father >> node;
			if (pos == 0)
				tree->addRightSubTreeByIndex (&nodes[node], father);
			else
				tree->addLeftSubTreeByIndex (&nodes[node], father);


			//Sleep (1000);
			//system ("cls");
			//tree->show (cout);
		} else {
			if (K == 0)
				tree->TreePreorderTraversal ();
			else
				tree->BiPreorderTraversal ();
		}
		//cout<<tree;

	}
	return 0;
}
