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
	void setChild (TreeNode* Node) {	 //无孩子的情况
		if (_firstChild == nullptr) {
			_firstChild = Node;
			Node->setParenet (this);
		}
		//第一个孩子的索引值大于Node的索引值的情况
		else if (Node->getIndex () < _firstChild->getIndex ()) {
			Node->setParenet (this);
			Node->setNextSibling (_firstChild);
			_firstChild = Node;
		} else {
			//找到所有的孩子中索引值不小于Node的最大节点
			TreeNode<T>* pTempNode = _firstChild;
			while (pTempNode->getNextSibling () != nullptr &&
				pTempNode->getNextSibling ()->getIndex () < Node->getIndex ())
				pTempNode = pTempNode->getNextSibling ();
			//该节点无下一个兄弟节点
			if (pTempNode->getNextSibling () == nullptr) {
				pTempNode->setNextSibling (Node);
				Node->setParenet (this);
			} else {  //该节点在两个节点之间
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

	/*查询子树的叶子数*/

	int NodeLeavesCount (int leaves) {
		if (this->_leftChild != nullptr)
			leaves = this->_leftChild->NodeLeavesCount (leaves);

		if (this->_rightChild != nullptr)
			leaves = this->_rightChild->NodeLeavesCount (leaves);

		if (this->getLChild () == nullptr && this->getRChild () == nullptr)
			leaves++;

		return leaves;
	}

	/*查询二叉子树的节点数(包括自己)*/

	int BiNodeChildrenCount () {
		int biCnt = 0;

		if (this->_leftChild != nullptr)
			biCnt += this->_leftChild->BiNodeChildrenCount ();

		if (this->_rightChild != nullptr)
			biCnt += this->_rightChild->BiNodeChildrenCount ();

		biCnt++;
		return biCnt;
	}

	/*查询子树的节点数(包括自己)*/

	int TreeNodeChildrenCount () {
		int tiCnt = 0;

		if (this->_firstChild != nullptr)
			tiCnt += this->_firstChild->TreeNodeChildrenCount ();

		if (this->_nextSibling != nullptr)
			tiCnt += this->_nextSibling->TreeNodeChildrenCount ();

		tiCnt++;
		return tiCnt;
	}

	/*删除二叉子树的所有节点*/

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

	/*删除子树的所有节点*/

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

	//加入0号根节点后，处理二叉树应该有一定的规律
	void ConvertToForest () {
		TreeNode<T>* pLeftNode = this->getLChild ();

		//this节点的右孩子节点或者没有或者已经处理过了，若无左孩子节点，返回即可
		if (pLeftNode == nullptr)
			return;

		//处理左孩子，左孩子的右孩子节点，以及左孩子的右孩子的右孩子节点等
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

	//应该也是递归
	void ConvertToBTree () {
		TreeNode<T>* pFirstChild = this->getFirstChild ();
		TreeNode<T>* pNextSibling = this->getNextSibling ();

		//长子为左孩子，下一个兄弟为右孩子
		this->setLChild (pFirstChild);
		this->setRChild (pNextSibling);
		this->setFirstChild (nullptr);
		this->setNextSibling (nullptr);

		if (pFirstChild != nullptr) pFirstChild->ConvertToBTree ();

		if (pNextSibling != nullptr) pNextSibling->ConvertToBTree ();
	}


//private:
	int _index;	 //索引
	T _data;	 //值

	TreeNode* _parent;		 //父亲节点
	TreeNode* _leftChild;	 //左孩子节点
	TreeNode* _rightChild;	 //右孩子节点
	TreeNode* _firstChild;	 //长子节点
	TreeNode* _nextSibling;	 //兄弟节点
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

	bool IsTreeEmpty ();	 //树是否为空
	bool IsTreeFull ();	 //树的容量是否已满

	//search
	TreeNode<T>* getBiNodeByIndex (int index);  //通过索引搜索节点
	TreeNode<T>* getTreeNodeByIndex (int index);
	int getLeaves ();	 //获取树的叶子数
	int getHeight ();	 //获取树的高度(包含根节点)
	int getWidth ();		 //获取树的宽度(包含根节点)
	int getNowBiSize ();	 //获取树现在的节点数（包含根节点）
	int getNowTreeSize ();
	int getMaxSize ();  //获取树的最大节点数

	bool addLeftSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		_state = BTREE;
		TreeNode<T>* tempNode;
		tempNode = _root->BiNodeSearch (searchIndex);  //通过索引找到该节点

		tempNode->setLChild (pNode);
		_size += pNode->BiNodeChildrenCount ();
		return true;
	}

	bool addRightSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		TreeNode<T>* tempNode;
		tempNode = _root->BiNodeSearch (searchIndex);  //通过索引找到该节点

		tempNode->setRChild (pNode);

		return true;
	}

	bool addSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {
		if (_root == nullptr)
			return false;
		_state = FOREST;

		TreeNode<T>* tempNode;
		tempNode = _root->TreeNodeSearch (searchIndex);	//通过索引找到该节点
		tempNode->setChild (pNode);
		return true;
	}

	bool addChildNodeByIndex (int newIndex, T data, int searchIndex) {
		if (_root == nullptr)
			return false;

		TreeNode<T>* tempNode;
		tempNode = _root->TreeNodeSearch (searchIndex);	//通过索引找到该节点

		if (tempNode != nullptr) {
			//cout << tempNode->getIndex() << endl;
			return addChildNodeByNode (newIndex, data, tempNode);
		}

		return false;
	}

	bool addChildNodeByNode (int index, T data, TreeNode<T>* pNode) {
		TreeNode<T>* pNodeCopy = pNode;	 //做pNode的副本，防止pNode的被意外修改

		if (IsTreeFull ()) return false;
		//得到长子节点
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
			//找到索引值不小于index的最大节点
			TreeNode<T>* pTempNode = pFirstChildNode;
			while (pTempNode->getNextSibling () != nullptr &&
				pTempNode->getNextSibling ()->getIndex () < index)
				pTempNode = pTempNode->getNextSibling ();
			//该节点无下一个兄弟节点
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
			//找到nodeB的前一个兄弟节点
			while (tempNode->getNextSibling () != nodeB)
				tempNode = tempNode->getNextSibling ();

			tempNode->setNextSibling (nodeB->getNextSibling ());
			nodeB->setNextSibling (nullptr);
			nodeB->setParenet (nodeA);
			nodeA->setChild (nodeB);
		} else if (a > b) {
			//b是长子,将b的下一个兄弟节点作为长子
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
				cout << "deleteTreeNodeByIndex():" << index << "是根节点不能删除" << endl;
				return false;
			}
			return deleteTreeNodeByNode (deleteNode);
		}
		return false;
	}

	bool deleteTreeNodeByNode (TreeNode<T>* pNode) {
		if (pNode != nullptr) {
			//先看是否有孩子节点
			TreeNode<T>* pFirstChildNode = pNode->getFirstChild ();

			//有孩子节点先将孩子节点都作为0号节点的子孩子
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

			//处理完孩子节点再处理兄弟链表
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


//////////////////打印
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
				cout << "│ ";
			} else {
				cout << "  ";
			}

		}

		if (rootin->getNextSibling () != NULL) {
			cout << "├ ";

		} else {
			cout << "└ ";
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

	char charmap[1000][1000]; //用于存放要打印的横向二叉树 

	//void ConvertToForest () { _root->ConvertToForest (); }
	//void ConvertToBTree () { _root->ConvertToBTree (); }
	TreeNode<T>* _root;	 //树根节点
	int _size;			 //当前树的节点数(不包括根节点)
	int _maxSize;		 //树的最大节点数(不包括根节点)
};


#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#pragma warning(disable : 4996)
int main () {
	FILE* fd  = freopen ("in2.txt", "r", stdin);

	int i, j, K, M, N, Q, node, rootIndex, pos;
	Tree<int>* tree = new Tree<int> (5302, 0, 0);

	//节点初始化
	TreeNode<int>* nodes = new TreeNode<int>[5302];
	for (i = 0; i < 5102; i++) {
		nodes[i].setIndex (i);
	}

	cin >> K >> M >> N;

	//K=0初始化森林
	if (K == 0) {
		//M为树根节点的个数
		int* roots = new int[M];
		int A, B;
		for (i = 0; i < M; i++) cin >> roots[i];

		//A节点有B个孩子
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
		//初始化二叉树
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

	//进行树的操作
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
