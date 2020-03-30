#include<queue>
#include<queue>
#include <vector>
#include<iostream>
using namespace std;

//ǰ������
template<class T>
class Forest;

template<class T>
class TreeNode;

template<class T>
class Tree;

template<class T>
class BinaryTreeNode;

template<class T>
class BinaryTreeNode;


/*BinaryTree*/
template<class T>
class BinaryTreeNode {
public:
	BinaryTreeNode (const T& theElement) :
		element (theElement),
		leftNode (nullptr),
		rightNode (nullptr) {}

	T element;
	BinaryTreeNode<T>* leftNode;
	BinaryTreeNode<T>* rightNode;
};
template<class T>
class BTree {
public:
	BinaryTreeNode<T>* m_pBinaryTree;
	BTree ();
	BTree (BinaryTreeNode<T>* pRoot, size_t pSize);
	BTree (const Forest<T>& pForest) {
		if (pForest.size () == 0)
			return;

		//��ÿһ����ת���ɶ�������Ȼ��������
		BinaryTreeNode<T>* pRoot = (new BTree<T> (*pForest.at (0)))->root ();
		BinaryTreeNode<T>* pCurRoot = pRoot;
		BinaryTreeNode<T>* pNextRoot = nullptr;
		size_t pSize = pForest.at (0)->size ();

		for (int i = 1; i < pForest.size (); ++i) {
			pSize += pForest.at (i)->size ();
			pNextRoot = (new BTree<T> (*pForest.at (i)))->root ();
			pCurRoot->rightNode = pNextRoot;
			pCurRoot = pNextRoot;
		}

		_root = pRoot;
		_size = pSize;
	}
	BTree (const Tree<T>& pRoot) {
		treeToBinaryTree (nullptr, pRoot.root ());
		_root = m_pBinaryTree;
	}
	/************************************************************************/
	/*                           ���������                                  */
	/*                  �����ֵܽڵ�֮���һ����								*/
	/*                  ���нڵ�ֻ����������ߵ�һ�����ӵ�����                  */
	/************************************************************************/
	void treeToBinaryTree (BinaryTreeNode<T>* parentNode, TreeNode<T>* pNode) {
		if (pNode == nullptr)
			return;

		if (parentNode == nullptr) {
			parentNode = new BinaryTreeNode<T> (pNode->element);
			m_pBinaryTree = parentNode;
		}

		int pCnt = pNode->pSize;
		if (pCnt == 0)
			return;

		BinaryTreeNode<T>* lhsNode, * rhsNode;
		lhsNode = new BinaryTreeNode<T> (pNode->next[0]->element);
		parentNode->leftNode = lhsNode; //ֻ��������ߵ�һ���ڵ������

		//�ֵܽڵ�֮������
		for (int i = 0; i < pCnt - 1; ++i) {
			rhsNode = new BinaryTreeNode<T> (pNode->next[i + 1]->element);
			lhsNode->rightNode = rhsNode;
			lhsNode = rhsNode;
		}

		//�ݹ���ã������е��ֵܽڵ�ΪĿ��ڵ㣬�Զ�������ͬһλ�ýڵ�Ϊ���ڵ�
		lhsNode = parentNode->leftNode;
		for (int i = 0; i < pCnt; ++i) {
			treeToBinaryTree (lhsNode, pNode->next[i]);
			lhsNode = lhsNode->rightNode;
		}
	}
	~BTree ();
	size_t size () const;
	bool isEmpty () const;
	void initialize (T elements[], size_t pSize);
	void levelOrderPrint ();
	BinaryTreeNode<T>* root () const;

private:
	BinaryTreeNode<T>* _root;
	size_t _size;
};

template<class T>
BTree<T>::BTree () :
	_root (nullptr),
	_size (0) {}

template<class T>
BTree<T>::BTree (BinaryTreeNode<T>* pRoot, size_t pSize) :
	_root (pRoot),
	_size (pSize) {}


template<class T>
BTree<T>::~BTree () {
	queue<BinaryTreeNode<T>*> pQueue;
	if (_root != nullptr)
		pQueue.push (_root);

	while (!pQueue.empty ()) {
		BinaryTreeNode<T>* currentNode = pQueue.front ();
		pQueue.pop ();

		if (currentNode->leftNode != nullptr)
			pQueue.push (currentNode->leftNode);
		if (currentNode->rightNode != nullptr)
			pQueue.push (currentNode->rightNode);

		delete currentNode;
	}
}

template<class T>
BinaryTreeNode<T>* BTree<T>::root () const {
	return _root;
}

template<class T>
bool BTree<T>::isEmpty () const {
	return !_size;
}

template<class T>
size_t BTree<T>::size () const {
	return _size;
}

template<class T>
void BTree<T>::initialize (T elements[], size_t pSize) {
	if (pSize <= 0)
		return;

	queue<BinaryTreeNode<T>*> pQueue;
	_root = new BinaryTreeNode<T> (elements[0]);
	pQueue.push (_root);

	size_t i = 1;
	while (i < pSize) {
		BinaryTreeNode<T>* pCurNode = pQueue.front ();
		pQueue.pop ();
		pCurNode->leftNode = new BinaryTreeNode<T> (elements[i++]);
		pQueue.push (pCurNode->leftNode);

		if (i == pSize)
			break;
		pCurNode->rightNode = new BinaryTreeNode<T> (elements[i++]);
		pQueue.push (pCurNode->rightNode);
	}
}

template<class T>
void BTree<T>::levelOrderPrint () {
	queue<BinaryTreeNode<T>*> pQueue;
	if (_root != nullptr)
		pQueue.push (_root);

	while (!pQueue.empty ()) {
		BinaryTreeNode<T>* targetNode = pQueue.front ();
		pQueue.pop ();

		//	cout << "parent: " << targetNode->element << endl;
		//	cout << "children: ";
		if (targetNode->leftNode != nullptr) {
			pQueue.push (targetNode->leftNode);
			//cout << targetNode->leftNode->element << " ";
		}
		if (targetNode->rightNode != nullptr) {
			pQueue.push (targetNode->rightNode);
			//cout << targetNode->rightNode->element << " ";
		}
		//	cout << endl << endl;
		cout << targetNode->element << " ";
	}
}



/*Forest*/
template<class T>
class Forest {
private:
	vector<Tree<T>* > _trees;
public:
	/*��һ�������г�ʼ����ɭ��*/
	Forest (BTree<T>& pRoot) {
		vector<Tree<T>* > m_pTreeVector;
		vector<BinaryTreeNode<T>*> m_pBinaryTreeVector;

		//��ֶ�����
		BinaryTreeNode<T>* pNode = pRoot.root ();
		while (pNode != nullptr) {
			BinaryTreeNode<T>* pRightNode = pNode->rightNode;
			pNode->rightNode = nullptr;
			m_pBinaryTreeVector.push_back (pNode);
			pNode = pRightNode;
		}

		//��ÿһ�ö�������ת������
		for (int i = 0; i < m_pBinaryTreeVector.size (); ++i) {
			_trees.push_back (new Tree<T> (m_pBinaryTreeVector.at (i)));
		}

		//_trees = m_pTreeVector;
	}
	~Forest () {}

	size_t size () const { return _trees.size (); };
	unique_ptr<Tree<T> > getTree (int index)const {
		if (index >= _trees.size ()) {
			return nullptr;
		}

		return new Tree<T> (_trees[index]);
	}
	Tree<T>* at (int index)const {
		if (index >= _trees.size ()) {
			return nullptr;
		}

		return  _trees.at (index);
	}
};






/*Tree*/
template<class T>
class TreeNode {
public:
	TreeNode (const T& theElement, size_t pCnt = 0) :
		element (theElement),
		pSize (pCnt) {
		next = new TreeNode<T> * [pCnt];
		for (size_t i = 0; i < pCnt; ++i)
			next[i] = nullptr;
		if (pCnt == 0)
			next = nullptr;
	}


	~TreeNode () {
		delete[]next;
	}

	void setSize (size_t pCnt) {
		delete[]next;
		next = new TreeNode<T> * [pCnt];
		pSize = pCnt;
	}

	size_t pSize;
	T element;
	TreeNode<T>** next;
};
template<class T>
class Tree {
public:
	Tree (TreeNode<T>* pRoot = nullptr);
	~Tree ();

	size_t size () const;
	TreeNode<T>* root () const;
	void levelOrderPrint () const;

	BinaryTreeNode<T>* t_root = nullptr;

	Tree (BinaryTreeNode<T>* pRoot) {
		t_root = pRoot;
		binaryTreeToTree<T> (nullptr, pRoot);
	}


	/************************************************************************/
	/* ������->��															*/
	/* 1.��ĳ���ڵ������ӣ����ýڵ��������ӣ����ӵ��Һ��ӣ�					*/
	/*   ���ӵ��Һ��ӵ��Һ��ӣ����ӵ��Һ��ӵ��Һ��ӵ��Һ���....����			*/
	/* 2.ɾ��ԭ�е���Щ���У��ڵ����Һ��ӵ�����									*/
	/************************************************************************/
	/* ע�����ڵ㣬���ڵ���Һ��ӣ����ڵ���Һ��ӵ��Һ���...Ҳ���ڲ���1��          */
	/************************************************************************/

	/*parentNode: �������д�ʱ�������ڵ�*/
	/*pNode: ���д�ʱ�������ڵ�*/
	template<class T>
	void binaryTreeToTree (TreeNode<T>* parentNode, BinaryTreeNode<T>* pNode) {
		if (pNode == nullptr)
			return;

		//�ж��Ƿ��ǣ�ע�������
		bool isRightNode = false;
		BinaryTreeNode<T>* targetNode = t_root;
		while (targetNode) {
			if (targetNode == pNode) {
				isRightNode = true;
				break;
			}
			targetNode = targetNode->rightNode;
		}

		//�������иýڵ�ĺ�����������1�����߸�����
		BinaryTreeNode<T>* pLeftNode = pNode->leftNode;
		int pCnt = 0;
		while (pLeftNode) {
			++pCnt;
			pLeftNode = pLeftNode->rightNode;
		}

		//��ע�������
		if (isRightNode && pNode->rightNode != nullptr)
			++pCnt;

		if (parentNode == nullptr) {
			parentNode = new TreeNode<T> (pNode->element, pCnt);
			_root = parentNode;  //��¼���ĸ��ڵ�
		} else
			parentNode->setSize (pCnt);  //���ú�������

		pLeftNode = pNode->leftNode;
		pCnt = 0;
		while (pLeftNode) {
			//Ϊ�����½ڵ���Ӻ��ӣ��ò���1���ߵĶ���������ʼ�����ڵ㣩
			TreeNode<T>* pChildNode = new TreeNode<T> (pLeftNode->element);
			parentNode->next[pCnt] = pChildNode;
			pLeftNode = pLeftNode->rightNode;
			++pCnt;
		}

		//��ע�������
		if (isRightNode && pNode->rightNode != nullptr) {
			TreeNode<T>* pChildNode = new TreeNode<T> (pNode->rightNode->element);
			parentNode->next[pCnt++] = pChildNode;
		}

		//���亢�ӽ��еݹ���ú���
		pLeftNode = pNode->leftNode;
		for (int i = 0; i < pCnt; ++i) {
			binaryTreeToTree (parentNode->next[i], pLeftNode);
			if (isRightNode && i == pCnt - 1) //��ע�������
				pLeftNode = pNode->rightNode;
			else
				pLeftNode = pLeftNode->rightNode;
		}
	}

private:
	void destroy (TreeNode<T>* pNode);
private:
	TreeNode<T>* _root;
	size_t _size;
};

template<class T>
Tree<T>::Tree (TreeNode<T>* pRoot) :
	_root (pRoot),
	_size (0) {
	queue<TreeNode<T>*> pQueue;
	if (_root != nullptr) {
		pQueue.push (_root);
		++_size;
	}

	while (!pQueue.empty ()) {
		TreeNode<T>* targetNode = pQueue.front ();
		pQueue.pop ();
		_size += targetNode->pSize;
		for (int i = 0; i < targetNode->pSize; ++i)
			pQueue.push (targetNode->next[i]);
	}
}

template<class T>
Tree<T>::~Tree () {
	destroy (_root);
}


template<class T>
void Tree<T>::destroy (TreeNode<T>* pNode) {
	for (int i = 0; i < pNode->pSize; ++i) {
		destroy (pNode->next[i]);
	}
	delete pNode;
}

template<class T>
size_t Tree<T>::size () const {
	return _size;
}


template<class T>
TreeNode<T>* Tree<T>::root () const {
	return _root;
}


template<class T>
void Tree<T>::levelOrderPrint () const {
	queue<TreeNode<T>*> pQueue;
	if (_root != nullptr)
		pQueue.push (_root);

	while (!pQueue.empty ()) {
		TreeNode<T>* targetNode = pQueue.front ();
		pQueue.pop ();

		cout << "parent: " << targetNode->element << endl;
		cout << "children: ";
		for (size_t i = 0; i < targetNode->pSize; ++i) {
			pQueue.push (targetNode->next[i]);
			cout << targetNode->next[i]->element << " ";
		}
		cout << endl << endl;
		//	cout << targetNode->element << " ";
	}
}





//main����
#include <iostream>
#include <vector>
using namespace std;

#define LL int

#define DEBUG
#ifdef DEBUG
#define dprintf printf
#else
#define dprintf  /\
/printf
#endif

int K, M, N;
int main () {
	//int initElement[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	//BTree<int> binaryTree;
	//binaryTree.initialize (initElement, 20);
	//binaryTree.levelOrderPrint ();
	//cout << endl;


	//Forest<int> f (binaryTree);

	//BTree<int> t (f);
	//t.levelOrderPrint ();


	//������ ���ڵ���Ŀ  ������Ŀ
	scanf ("%d %d %d", &K, &M, &N);

	if (K == 1) {
		//������
		dprintf ("������\n");
		if (M != 1) {
			dprintf ("�����������ж�����ڵ�\n");
			return 0;
		}




	} else {
		//ɭ��
		dprintf ("ɭ��\n");
		for (int i = 0; i < M; i++) {

		}

	}



	return 0;
}
