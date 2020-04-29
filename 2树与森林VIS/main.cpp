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
	void setChild (Node* nodein) {	 //无孩子的情况
		if (_firstChild == nullptr) {
			_firstChild = nodein;
			nodein->setParenet (this);
		}
		//第一个孩子的索引值大于Node的索引值的情况
		else if (nodein->getIndex () < _firstChild->getIndex ()) {
			nodein->setParenet (this);
			nodein->setNextSibling (_firstChild);
			_firstChild = nodein;
		} else {
			//找到所有的孩子中索引值不小于Node的最大节点
			Node<T>* pTempNode = _firstChild;
			while (pTempNode->getNextSibling () != nullptr &&
				pTempNode->getNextSibling ()->getIndex () < nodein->getIndex ())
				pTempNode = pTempNode->getNextSibling ();
			//该节点无下一个兄弟节点
			if (pTempNode->getNextSibling () == nullptr) {
				pTempNode->setNextSibling (nodein);
				nodein->setParenet (this);
			} else {  //该节点在两个节点之间
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

	/*查询子树的叶子数*/

	int NodeLeavesCount (int leaves) {
		if (this->_left != nullptr)
			leaves = this->_left->NodeLeavesCount (leaves);

		if (this->_right != nullptr)
			leaves = this->_right->NodeLeavesCount (leaves);

		if (this->getLChild () == nullptr && this->getRChild () == nullptr)
			leaves++;

		return leaves;
	}

	/*查询二叉子树的节点数(包括自己)*/

	int BiNodeChildrenCount () {
		int biCnt = 0;

		if (this->_left != nullptr)
			biCnt += this->_left->BiNodeChildrenCount ();

		if (this->_right != nullptr)
			biCnt += this->_right->BiNodeChildrenCount ();

		biCnt++;
		return biCnt;
	}

	/*查询子树的节点数(包括自己)*/

	int NodeChildrenCount () {
		int tiCnt = 0;

		if (this->_firstChild != nullptr)
			tiCnt += this->_firstChild->NodeChildrenCount ();

		if (this->_sibling != nullptr)
			tiCnt += this->_sibling->NodeChildrenCount ();

		tiCnt++;
		return tiCnt;
	}

	/*删除二叉子树的所有节点*/

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

	/*删除子树的所有节点*/

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
	//二叉树转化为森林
	//二叉树节点的左子节点的右节点链条转化为子节点链表
	void _convertToForest () {
		Node<T>* pLeftNode = this->getLChild ();
		if (pLeftNode == nullptr)return;

		Node<T>* pPNode = pLeftNode;
		Node<T>* pRNode;
		while (pPNode != nullptr) {
			pRNode = pPNode->getRChild ();
			pPNode->setRChild (nullptr);//第一次是左子节点转化为第一个子节点

			this->setChild (pPNode);
			pPNode->_convertToForest ();

			pPNode = pRNode;
		}
	}

	//森林转化为二叉树
	//第一个子节点是左子节点，兄弟节点是右节点链条
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
	int _index;	 //索引
	T _data;	 //值

	Node* _parent;		 //父亲节点
	
	//二叉树
	Node* _left;	 //左子节点
	Node* _right;	 //右子节点

	//森林
	Node* _firstChild;	 //子节点链表头部
	Node* _sibling;	 //兄弟节点
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

	bool IsTreeEmpty ();	 //树是否为空
	bool IsTreeFull ();	 //树的容量是否已满

	//搜索
	Node<T>* getBiNodeByIndex (int index);  //通过索引搜索节点
	Node<T>* getNodeByIndex (int index);
	
	//基本信息
	int getLeaves ();	 //获取树的叶子数
	int getHeight ();	 //获取树的高度(包含根节点)
	int getWidth ();		 //获取树的宽度(包含根节点)
	int getNowBiSize ();	 //获取树现在的节点数（包含根节点）
	int getNowTreeSize ();
	int getMaxSize ();  //获取树的最大节点数

	bool addLeftSubTreeByIndex (Node<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		_state = BTREE;
		Node<T>* tempNode;
		tempNode = _root->BNodeSearch (searchIndex);  //通过索引找到该节点

		tempNode->setLChild (pNode);
		_size += pNode->BiNodeChildrenCount ();
		return true;
	}

	bool addRightSubTreeByIndex (Node<T>* pNode, int searchIndex) {
		if (_root == nullptr) return false;
		Node<T>* tempNode;
		tempNode = _root->BNodeSearch (searchIndex);  //通过索引找到该节点

		tempNode->setRChild (pNode);

		return true;
	}

	bool addSubTreeByIndex (Node<T>* pNode, int searchIndex) {
		if (_root == nullptr)
			return false;
		_state = FOREST;

		Node<T>* tempNode;
		tempNode = _root->NodeSearch (searchIndex);	//通过索引找到该节点
		tempNode->setChild (pNode);
		return true;
	}

	bool addChildNodeByIndex (int newIndex, T data, int searchIndex) {
		if (_root == nullptr)
			return false;

		Node<T>* tempNode;
		tempNode = _root->NodeSearch (searchIndex);	//通过索引找到该节点

		if (tempNode != nullptr) {
			//cout << tempNode->getIndex() << endl;
			return addChildNodeByNode (newIndex, data, tempNode);
		}

		return false;
	}

	bool addChildNodeByNode (int index, T data, Node<T>* pNode) {
		Node<T>* pNodeCopy = pNode;	 //做pNode的副本，防止pNode的被意外修改

		if (IsTreeFull ()) return false;
		//得到长子节点
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
			//找到索引值不小于index的最大节点
			Node<T>* pTempNode = pFirstChildNode;
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
		Node<T>* nodeA, * nodeB, * tempNode;

		nodeA = _root->NodeSearch (a);
		nodeB = _root->NodeSearch (b);
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

	//森林删除节点
	bool forestDeleteNodeByIndex (int index) {
		Node<T>* deleteNode = _root->NodeSearch (index);

		if (deleteNode != nullptr) {
			if (deleteNode == _root) {
				cout << "forestDeleteNodeByIndex():" << index << "是根节点不能删除" << endl;
				return false;
			}
			return deleteNodeIn (deleteNode);
		}
		return false;
	}

	bool deleteNodeIn (Node<T>* pNode) {
		if (pNode != nullptr) {
			//孩子节点
			Node<T>* first_child = pNode->getFirstChild ();
			//将孩子节点当做根节点的孩子
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

			//兄弟  在父节点的子节点链表中
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


//////////////////打印
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

	char charmap[1000][1000]; //用于存放要打印的横向二叉树 

	//void toForest () { _root->toForest (); }
	//void toBTree () { _root->toBTree (); }
	Node<T>* _root;	 //树根节点
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
	Node<int>* nodes = new Node<int>[5302];
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

		//A节点有B个孩子,数组中每个节点都有自己的地址，通过地址进行节点关系的保存
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
	} else {//初始化二叉树
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

	//显示图形界面，接受用户操作
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
			cout << "*现在是二叉树模式\n";
		}else {
			cout << "*现在是森林模式\n";
		}
		cout << R"(*请输入指令：
*  1 father node 添加节点
*  2 father node 删除节点
*  3 a b 添加边
*  4 树与森林转换
*  5 pos(0右1左) father node 二叉树添加子节点)";
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
