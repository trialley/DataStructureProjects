#include <iostream>
#include <queue>
#include <vector>
using namespace std;

/*template <typename K,typename V>
class TNode {
private:
	TNode* _parent;//指向父节点
	TNode* _left;//指向左子节点
	TNode* _right;//指向右子节点

	//树的部分采用链表表示
	TNode* _sub;//指向树的第一个子节点
	TNode* _next;//指向树的兄弟节点

	int _index;//OJ上输入的索引
	V _data;//数据
public:
	TNode ()
	TNode (int index, V data)
	~TNode ()

	int getIndex ()
	V getData ()
	TNode<K, V>* getParent ()
	TNode<K, V>* getLChild ()
	TNode<K, V>* getRChild ()
	TNode<K, V>* getFirstChild ()
	TNode<K, V>* getNextSibling ()

	int BPreorder ()
	int TPreorder ()
	void BTreeToTree ()
	void TreeToBTree ()

	void setIndex (int index)
	void setData (V data)
	void setParenet (TNode* Node)
	void setLChild (TNode* Node)
	void setRChild (TNode* Node)
	void setFirstChild (TNode* Node)
	void setNextSibling (TNode* Node)
	void setChild (TNode* Node)
	TNode<K, V>* BiNodeSearch (int index)
	TNode<K, V>* Tsearch (int index)
	int NodeLeavesCount (int leaves)
	int getBTChildrenNum ()
	int deleteBAll ()
	int deleteTNode ()
};
*/
template <typename K,typename V>
class TNode {
private:
	TNode* _parent;//指向父节点
	TNode* _left;//指向左子节点
	TNode* _right;//指向右子节点

	/*树的部分采用链表表示*/
	TNode* _sub;//指向树的第一个子节点
	TNode* _next;//指向树的兄弟节点
	
	int _index;//OJ上输入的索引
	V _data;//数据
public:
	TNode () : _index (0), _parent (0), _left (0), _right (0), _sub (0), _next (0), _data (0) {}
	TNode (int index, V data) : _index (index), _data (data), _parent (0), _left (0), _right (0), _sub (0), _next (0) {}
	~TNode () {
		if (_left != nullptr) {
			_left->deleteBAll ();
			_left = nullptr;
		}

		if (_right != nullptr) {
			_right->deleteBAll ();
			_right = nullptr;
		}

		if (_sub != nullptr) {
			_sub->deleteTNode ();
			_sub = nullptr;
		}

		if (_next != nullptr) {
			_next->deleteTNode ();
			_next = nullptr;
		}
		_parent = nullptr;
	}


	int getIndex () { return _index; }
	V getData () { return _data; }
	TNode<K,V>* getParent () { return _parent; }
	TNode<K,V>* getLChild () { return _left; }
	TNode<K,V>* getRChild () { return _right; }
	TNode<K,V>* getFirstChild () { return _sub; }
	TNode<K,V>* getNextSibling () { return _next; }

	void setIndex (int index) { _index = index; }
	void setData (V data) { _data = data; }
	void setParenet (TNode* Node) { _parent = Node; }
	void setLChild (TNode* Node) {
		_left = Node;
		if (Node != nullptr) Node->setParenet (this);
	}
	void setRChild (TNode* Node) {
		_right = Node;
		if (Node != nullptr) Node->setParenet (this);
	}
	void setFirstChild (TNode* Node) { _sub = Node; }
	void setNextSibling (TNode* Node) { _next = Node; }
	void setChild (TNode* Node) {
		if (_sub == nullptr) {
			_sub = Node;
			Node->setParenet (this);
		}

		else if (Node->getIndex () < _sub->getIndex ()) {
			Node->setParenet (this);
			Node->setNextSibling (_sub);
			_sub = Node;
		} else {
			TNode<K,V>* ptemp_node = _sub;
			while (ptemp_node->getNextSibling () != nullptr &&
				ptemp_node->getNextSibling ()->getIndex () < Node->getIndex ())
				ptemp_node = ptemp_node->getNextSibling ();
			if (ptemp_node->getNextSibling () == nullptr) {
				ptemp_node->setNextSibling (Node);
				Node->setParenet (this);
			} else {
				Node->setParenet (this);
				Node->setNextSibling (ptemp_node->getNextSibling ());
				ptemp_node->setNextSibling (Node);
			}
		}
	}

	TNode<K,V>* BiNodeSearch (int index) {
		TNode<K,V>* temp_node = nullptr;
		if (_index == index) {
			return this;
		}
		if (_left != nullptr) {
			temp_node = _left->BiNodeSearch (index);
			if (temp_node != nullptr) {
				return temp_node;
			}
		}

		if (_right != nullptr) {
			temp_node = _right->BiNodeSearch (index);
			if (temp_node != nullptr) {
				return temp_node;
			}
		}
		return nullptr;
	}

	TNode<K,V>* Tsearch (int index) {
		TNode<K,V>* temp_node = nullptr;

		if (_index == index) {
			return this;
		}
		if (_sub != nullptr) {
			temp_node = _sub->Tsearch (index);
			if (temp_node != nullptr) {
				return temp_node;
			}
		}

		if (_next != nullptr) {
			temp_node = _next->Tsearch (index);
			if (temp_node != nullptr) {
				return temp_node;
			}
		}

		return nullptr;
	}

	int NodeLeavesCount (int leaves) {
		if (this->_left != nullptr)
			leaves = this->_left->NodeLeavesCount (leaves);

		if (this->_right != nullptr)
			leaves = this->_right->NodeLeavesCount (leaves);

		if (this->getLChild () == nullptr && this->getRChild () == nullptr)
			leaves++;

		return leaves;
	}

	int getBTChildrenNum () {
		int biCnt = 0;

		if (this->_left != nullptr)
			biCnt += this->_left->getBTChildrenNum ();

		if (this->_right != nullptr)
			biCnt += this->_right->getBTChildrenNum ();

		biCnt++;
		return biCnt;
	}


	int deleteBAll () {
		int Times = 0;
		if (this->_left != nullptr) {
			Times += this->_left->deleteBAll ();
			this->_left = nullptr;
		}

		if (this->_right != nullptr) {
			Times += this->_right->deleteBAll ();
			this->_right = nullptr;
		}

		Times++;
		delete this;
		return Times;
	}

	int deleteTNode () {
		int Times = 0;
		if (this->_sub != nullptr) {
			Times += this->_sub->deleteTNode ();
			this->_sub = nullptr;
		}

		if (this->_next != nullptr) {
			Times += this->_next->deleteTNode ();
			this->_next = nullptr;
		}

		Times++;
		delete this;
		return Times;
	}

	int BPreorder () {
		int res = this->getIndex ();
		if (this->getLChild () != nullptr)
			res ^= this->getLChild ()->BPreorder ();
		if (this->getRChild () != nullptr)
			res ^= this->getRChild ()->BPreorder ();
		return res;
	}

	int TPreorder () {
		int res = this->getIndex ();
		if (this->getFirstChild () != nullptr)
			res ^= this->getFirstChild ()->TPreorder ();
		if (this->getNextSibling () != nullptr)
			res ^= this->getNextSibling ()->TPreorder ();
		return res;
	}

	void BTreeToTree () {
		TNode<K,V>* pLeftNode = this->getLChild ();
		if (pLeftNode == nullptr)
			return;
		TNode<K,V>* pnode_in = pLeftNode;
		TNode<K,V>* pRNode;
		while (pnode_in != nullptr) {
			pRNode = pnode_in->getRChild ();
			pnode_in->setRChild (nullptr);
			this->setChild (pnode_in);
			pnode_in->BTreeToTree ();
			pnode_in = pRNode;
		}
	}

	void TreeToBTree () {
		TNode<K,V>* pFirstChild = this->getFirstChild ();
		TNode<K,V>* pNextSibling = this->getNextSibling ();

		this->setLChild (pFirstChild);
		this->setRChild (pNextSibling);
		this->setFirstChild (nullptr);
		this->setNextSibling (nullptr);

		if (pFirstChild != nullptr) pFirstChild->TreeToBTree ();
		if (pNextSibling != nullptr) pNextSibling->TreeToBTree ();
	}
};


/*
template <typename K, typename V>
class Tree {
protected:
	TNode<K, V>* _root;	//指向根节点
	int _size;			//节点数目
public:
	Tree (K index, V data)
	Tree ()
	bool isEmpty ()
	bool setLeft (TNode<K, V>* node_in, K target)
	bool setRight (TNode<K, V>* node_in, K target)
	bool setSub (TNode<K, V>* node_in, K target)
	bool setSubTo (K nwe_target, V data, K target)
	void addEdge (int a, int b)
	void BTreeToTree ()
	void TreeToBTree ()
	bool deleteTreeNodeByIndex (int index)
	bool deleteTreeNodeByNode (TNode<K, V>* node_in)
	void BPreorder ()
	void TreePreorder ()
};*/
template <typename K, typename V>
class Tree {
public:
	Tree ( K index, V data) : _root (new TNode<K,V> (index, data)), _size (1) {}
	Tree () : _root (new TNode<K,V> (0, 0)), _size (1){}
	~Tree () {
		if (_root != nullptr) delete _root;
		_root = nullptr;
	}

	bool isEmpty () { return _root; }

	bool setLeft (TNode<K,V>* node_in, K target) {
		if (_root == nullptr) return false;
		TNode<K,V>* temp_node;
		temp_node = _root->BiNodeSearch (target);

		temp_node->setLChild (node_in);
		_size += node_in->getBTChildrenNum ();
		return true;
	}

	bool setRight (TNode<K,V>* node_in, K target) {
		if (_root == nullptr) return false;
		TNode<K,V>* temp_node;
		temp_node = _root->BiNodeSearch (target);

		temp_node->setRChild (node_in);

		return true;
	}

	bool setSub (TNode<K,V>* node_in, K target) {
		if (_root == nullptr)
			return false;

		TNode<K,V>* temp_node;
		temp_node = _root->Tsearch (target);
		temp_node->setChild (node_in);
		return true;
	}

	bool setSubTo (K nwe_target, V data, K target) {
		if (_root == nullptr)
			return false;

		TNode<K,V>* temp_node;
		temp_node = _root->Tsearch (target);

		if (temp_node != nullptr) {
			return addChild (nwe_target, data, temp_node);
		}

		return false;
	}

	void addEdge (int a, int b) {
		TNode<K,V>* nodeA, * nodeB, * temp_node;

		nodeA = _root->Tsearch (a);
		nodeB = _root->Tsearch (b);
		if (a < b) {
			temp_node = nodeA;
			while (temp_node->getNextSibling () != nodeB)
				temp_node = temp_node->getNextSibling ();

			temp_node->setNextSibling (nodeB->getNextSibling ());
			nodeB->setNextSibling (nullptr);
			nodeB->setParenet (nodeA);
			nodeA->setChild (nodeB);
		} else if (a > b) {
			if (_root->getFirstChild () == nodeB) {
				TNode<K,V>* NextSiblingNode = nodeB->getNextSibling ();
				_root->setFirstChild (NextSiblingNode);
				nodeB->setNextSibling (nullptr);
				nodeA->setChild (nodeB);
			} else {
				temp_node = _root->getFirstChild ();
				while (temp_node->getNextSibling () != nodeB)
					temp_node = temp_node->getNextSibling ();
				temp_node->setNextSibling (nodeB->getNextSibling ());
				nodeB->setNextSibling (nullptr);
				nodeB->setParenet (nodeA);
				nodeA->setChild (nodeB);
			}
		}
	}

	bool deleteTreeNodeByIndex (int index) {
		TNode<K,V>* deleteNode = _root->Tsearch (index);

		if (deleteNode != nullptr) {
			if (deleteNode == _root) {
				cout << "deleteTreeNodeByIndex():" << index << "是根节点不能删除" << endl;
				return false;
			}
			return deleteTreeNodeByNode (deleteNode);
		}
		return false;
	}

	bool deleteTreeNodeByNode (TNode<K,V>* node_in) {
		if (node_in != nullptr) {
			TNode<K,V>* pFirstChildNode = node_in->getFirstChild ();
			if (pFirstChildNode != nullptr) {
				TNode<K,V>* pChildNode = pFirstChildNode;
				TNode<K,V>* pNextSiblingNode;
				while (pChildNode != nullptr) {
					pNextSiblingNode = pChildNode->getNextSibling ();
					_root->setChild (pChildNode);
					pChildNode = pNextSiblingNode;
				}
			}

			node_in->setFirstChild (nullptr);
			TNode<K,V>* pParentNode = node_in->getParent ();
			TNode<K,V>* pCNode = pParentNode->getFirstChild ();
			if (pCNode == node_in) {
				pParentNode->setFirstChild (node_in->getNextSibling ());
			} else {
				while (pCNode->getNextSibling () != node_in) {
					pCNode = pCNode->getNextSibling ();
				}
				pCNode->setNextSibling (node_in->getNextSibling ());
			}

			node_in->setNextSibling (nullptr);
			node_in->setFirstChild (nullptr);
			return true;
		}

		return false;
	}

	void BPreorder () {
		if (_root == nullptr) return;
		cout << _root->getLChild ()->BPreorder () << endl;
	}

	void TreePreorder () {
		if (_root == nullptr) return;

		TNode<K,V>* pFirstNode = _root->getFirstChild ();
		TNode<K,V>* pSiblingNode;

		int res = pFirstNode->getIndex ();
		if (pFirstNode->getFirstChild () != nullptr)
			res ^= pFirstNode->getFirstChild ()->TPreorder ();

		pSiblingNode = pFirstNode->getNextSibling ();
		cout << res << " ";

		while (pSiblingNode != nullptr) {
			res = pSiblingNode->getIndex ();
			if (pSiblingNode->getFirstChild () != nullptr)
				res ^= pSiblingNode->getFirstChild ()->TPreorder ();
			if (pSiblingNode->getNextSibling () != nullptr) {
				cout << res << " ";
			} else {
				cout << res;
			}
			pSiblingNode = pSiblingNode->getNextSibling ();
		}
		cout << endl;
	}

	void BTreeToTree () { _root->BTreeToTree (); }
	void TreeToBTree () { _root->TreeToBTree (); }

protected:
	TNode<K,V>* _root;//指向根节点
	int _size;//节点数目
};

#pragma warning(disable : 4996)
int main () {
	freopen ("in1.txt", "r", stdin);

	int i, j, K, M, N, Q, node, rootIndex, pos;
	Tree<int,int>* tree = new Tree<int,int>;

	TNode<int,int>* nodes = new TNode<int,int>[10000];
	for (i = 0; i < 5102; i++) {
		nodes[i].setIndex (i);
	}

	cin >> K >> M >> N;

	if (K == 0) {

		int* roots = new int[M];
		int A, B;
		for (i = 0; i < M; i++) cin >> roots[i];

		for (i = 0; i < N; i++) {
			cin >> A >> B;
			for (j = 0; j < B; j++) {
				cin >> node;
				nodes[A].setChild (&nodes[node]);
			}
		}

		for (i = 0; i < M; i++) {
			tree->setSub (&nodes[roots[i]], 0);
		}
	} else {
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
		tree->setLeft (&nodes[rootIndex], 0);
	}

	cin >> Q;
	int op, father, a, b;
	for (i = 0; i < Q; i++) {
		cin >> op;
		if (op == 1) {
			cin >> father >> node;
			if (father == -1) {
				tree->setSub (&nodes[node], 0);
			} else {
				tree->setSub (&nodes[node], father);
			}
		} else if (op == 2) {
			cin >> father >> node;
			tree->deleteTreeNodeByIndex (node);
		} else if (op == 3) {
			cin >> a >> b;
			tree->addEdge (a, b);
		} else if (op == 4) {
			if (K == 0) {
				tree->TreeToBTree ();
				K = 1;
			} else {
				tree->BTreeToTree ();
				K = 0;
			}
		} else if (op == 5) {
			cin >> pos >> father >> node;
			if (pos == 0)
				tree->setRight (&nodes[node], father);
			else
				tree->setLeft (&nodes[node], father);
		} else {
			if (K == 0)
				tree->TreePreorder ();
			else
				tree->BPreorder ();
		}
	}
	return 0;
}
