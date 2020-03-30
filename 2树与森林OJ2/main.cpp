#include<iostream>
#include <queue>
#include <vector>
using namespace std;

template <typename T>
class TreeNode {
public:
    TreeNode ();
    TreeNode (int index, T data);
    virtual ~TreeNode ();

    //get data
    int getIndex ();
    T getData ();

    TreeNode* getParent ();
    TreeNode* getLChild ();
    TreeNode* getRChild ();
    TreeNode* getFirstChild ();
    TreeNode* getNextSibling ();

    TreeNode* getInorderPrecursor ();      //获取中序前驱
    TreeNode* getInorderSubsequence ();    //获取中序后继

    //set data
    void setIndex (int index);
    void setData (T data);
    void setParenet (TreeNode* Node);
    void setLChild (TreeNode* Node);
    void setRChild (TreeNode* Node);
    void setFirstChild (TreeNode* Node);
    void setNextSibling (TreeNode* Node);

    void setChild (TreeNode* Node);

    //others
    TreeNode* BiNodeSearch (int index);      //通过索引搜索节点（以本节点作为根寻找树的某个节点）
    TreeNode* TreeNodeSearch (int index);
    int NodeLeavesCount (int leaves = 0);    //统计叶子数
    int NodeChildrenNodeHeigh ();            //统计以本节点作为根求树的高度
    int BiNodeChildrenCount ();              //统计二叉树子节点数(包含本节点)
    int TreeNodeChildrenCount ();
    int BTreeDeleteAll ();                       //删除二叉树节点
    int TreeNodeDelete ();

    //traversal
    int BiNodePreorderTraversal ();         //二叉树前序遍历
    int TreeNodePreorderTraversal ();

    void NodeInorderTraversal ();            //二叉树中序遍历
    void NodeSubsequentTraversal ();         //二叉树后序遍历

    void showTree ();
    void showBiTree ();

    void ConvertToTree ();
    void ConvertToBiTree ();

private:
    int _index;               //索引
    T _data;                  //值

    TreeNode* _parent;        //父亲节点
    TreeNode* _leftChild;     //左孩子节点
    TreeNode* _rightChild;    //右孩子节点
    TreeNode* _firstChild;     //长子节点
    TreeNode* _nextSibling;    //兄弟节点

    //struct NodeWidth<T> stNodeWidth;
};

template <typename T>
TreeNode<T>::TreeNode () {
    _index = 0;
    _data = 0;
    _parent = nullptr;
    _leftChild = nullptr;
    _rightChild = nullptr;
    _firstChild = nullptr;
    _nextSibling = nullptr;
}

template <typename T>
TreeNode<T>::TreeNode (int index, T data) {
    _index = index;
    _data = data;
    _parent = nullptr;
    _leftChild = nullptr;
    _rightChild = nullptr;
    _firstChild = nullptr;
    _nextSibling = nullptr;
}

template <typename T>
TreeNode<T>::~TreeNode () {
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

/*-----------------------getdata------------------------*/
template <typename T>
int TreeNode<T>::getIndex () {
    return _index;
}

template <typename T>
T TreeNode<T>::getData () {
    return _data;
}

template <typename T>
TreeNode<T>* TreeNode<T>::getParent () {
    return _parent;
}

template <typename T>
TreeNode<T>* TreeNode<T>::getLChild () {
    return _leftChild;
}

template <typename T>
TreeNode<T>* TreeNode<T>::getRChild () {
    return _rightChild;
}

template <typename T>
TreeNode<T>* TreeNode<T>::getFirstChild () {
    return _firstChild;
}

template <typename T>
TreeNode<T>* TreeNode<T>::getNextSibling () {
    return _nextSibling;
}

//获取中序前驱
template <typename T>
TreeNode<T>* TreeNode<T>::getInorderPrecursor () {
    /*
        condition 1: Node has left child.
        condition 2: Node hasn't left child,and it is its father right child.
        condition 3: Node hasn't left child,and it is its father left child.
    */
    /*condition 1:node has left child*/
    if (nullptr != this->getLChild ()) {
        TreeNode* tempNode = this->getLChild ();
        while (nullptr != tempNode->getRChild ()) {
            tempNode = tempNode->getRChild ();
        }
        return  tempNode;
    } else {
        TreeNode* fatherNode = this->getParent ();
        if (nullptr == fatherNode) {
            return nullptr;//it is root.
        }
        /*condition 2*/
        else if (fatherNode->getRChild () == this) {
            return fatherNode;
        }
        /*condition*/
        else {
            while (fatherNode->getParent ()->getRChild () != fatherNode) {
                fatherNode = fatherNode->getParent ();
                if (nullptr == fatherNode) {
                    return nullptr;//it is root;
                }
            }
            return fatherNode->getParent ();
        }
    }
    return nullptr;
}

//获取中序后继
template <typename T>
TreeNode<T>* TreeNode<T>::getInorderSubsequence () {
    /*
        condition 1: Node has right child.
        condition 2: Node hasn't right child,and it is its father left child.
        condition 3: Node hasn't right child,and it is its father right child.
    */
    /*condition 1*/
    if (nullptr != this->getRChild ()) {
        TreeNode* tempNode = this->getRChild ();
        while (nullptr != tempNode->getLChild ()) {
            tempNode = tempNode->getLChild ();
        }
        return  tempNode;
    }
    /*condition 2*/
    else {
        TreeNode* fatherNode = this->getParent ();
        if (nullptr == fatherNode)//it is root.
        {
            return nullptr;
        } else if (fatherNode->getLChild () == this) {
            return fatherNode;
        } else {
            while (fatherNode->getParent ()->getLChild () != fatherNode) {
                fatherNode = fatherNode->getParent ();
                if (nullptr == fatherNode) {
                    return nullptr;//it is root;
                }
            }
            return fatherNode->getParent ();
        }
    }
}


/*-----------------------setdata------------------------*/
template <typename T>
void TreeNode<T>::setIndex (int index) {
    _index = index;
}

template <typename T>
void TreeNode<T>::setData (T data) {
    _data = data;
}

template <typename T>
void TreeNode<T>::setParenet (TreeNode* Node) {
    _parent = Node;
}

template <typename T>
void TreeNode<T>::setLChild (TreeNode* Node) {
    _leftChild = Node;
    if (Node != nullptr)
        Node->setParenet (this);
}

template <typename T>
void TreeNode<T>::setRChild (TreeNode* Node) {
    _rightChild = Node;
    if (Node != nullptr)
        Node->setParenet (this);
}

template <typename T>
void TreeNode<T>::setFirstChild (TreeNode* Node) {
    _firstChild = Node;
}

template <typename T>
void TreeNode<T>::setNextSibling (TreeNode* Node) {
    _nextSibling = Node;
}

template <typename T>
void TreeNode<T>::setChild (TreeNode* Node) {
    //无孩子的情况
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
        }
        //该节点在两个节点之间
        else {
            Node->setParenet (this);
            Node->setNextSibling (pTempNode->getNextSibling ());
            pTempNode->setNextSibling (Node);
        }
    }
}

/*-----------------------others------------------------*/
template <typename T>
TreeNode<T>* TreeNode<T>::BiNodeSearch (int index) {
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

template <typename T>
TreeNode<T>* TreeNode<T>::TreeNodeSearch (int index) {
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

/*查询子树的高度(包括自己)*/
template <typename T>
int TreeNode<T>::NodeChildrenNodeHeigh () {
    int heightLeft = 0;
    int heightRight = 0;

    if (_leftChild != nullptr)
        heightLeft += _leftChild->NodeChildrenNodeHeigh ();

    if (_rightChild != nullptr)
        heightRight += _rightChild->NodeChildrenNodeHeigh ();

    if (heightRight > heightLeft)
        return ++heightRight;
    else
        return ++heightLeft;
}

/*查询子树的叶子数*/
template <typename T>
int TreeNode<T>::NodeLeavesCount (int leaves) {
    if (this->_leftChild != nullptr)
        leaves = this->_leftChild->NodeLeavesCount (leaves);

    if (this->_rightChild != nullptr)
        leaves = this->_rightChild->NodeLeavesCount (leaves);

    if (this->getLChild () == nullptr && this->getRChild () == nullptr)
        leaves++;

    return leaves;
}

/*查询二叉子树的节点数(包括自己)*/
template <typename T>
int TreeNode<T>::BiNodeChildrenCount () {
    int biCnt = 0;

    if (this->_leftChild != nullptr)
        biCnt += this->_leftChild->BiNodeChildrenCount ();

    if (this->_rightChild != nullptr)
        biCnt += this->_rightChild->BiNodeChildrenCount ();

    biCnt++;
    return biCnt;
}

/*查询子树的节点数(包括自己)*/
template <typename T>
int TreeNode<T>::TreeNodeChildrenCount () {

    int tiCnt = 0;

    if (this->_firstChild != nullptr)
        tiCnt += this->_firstChild->TreeNodeChildrenCount ();

    if (this->_nextSibling != nullptr)
        tiCnt += this->_nextSibling->TreeNodeChildrenCount ();

    tiCnt++;
    return tiCnt;

}

/*删除二叉子树的所有节点*/
template <typename T>
int  TreeNode<T>::BTreeDeleteAll () {
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
template <typename T>
int  TreeNode<T>::TreeNodeDelete () {
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
template <typename T>
int TreeNode<T>::BiNodePreorderTraversal () {

    //cout<<"Index:"<<this->getIndex()<<";Data:"<<this->getData()<<endl;
    int res = this->getIndex ();

    if (this->getLChild () != nullptr)
        res ^= this->getLChild ()->BiNodePreorderTraversal ();

    if (this->getRChild () != nullptr)
        res ^= this->getRChild ()->BiNodePreorderTraversal ();

    return res;
}

template <typename T>
void TreeNode<T>::showBiTree () {

    cout << "Index:" << this->getIndex () << ";Data:" << this->getData () << endl;

    if (this->getLChild () != nullptr)
        this->getLChild ()->showBiTree ();

    if (this->getRChild () != nullptr)
        this->getRChild ()->showBiTree ();

}

template <typename T>
void TreeNode<T>::showTree () {

    cout << "Index:" << this->getIndex () << ";Data:" << this->getData () << endl;

    if (this->getFirstChild () != nullptr)
        this->getFirstChild ()->showTree ();

    if (this->getNextSibling () != nullptr)
        this->getNextSibling ()->showTree ();

}

template <typename T>
void TreeNode<T>::NodeInorderTraversal () {

    if (this->getLChild () != nullptr)
        this->getLChild ()->NodeInorderTraversal ();

    cout << "Index:" << this->getIndex () << ";Data:" << this->getData () << endl;

    if (this->getRChild () != nullptr)
        this->getRChild ()->NodeInorderTraversal ();
}

template <typename T>
void TreeNode<T>::NodeSubsequentTraversal () {
    if (this->getLChild () != nullptr)
        this->getLChild ()->NodeSubsequentTraversal ();

    if (this->getRChild () != nullptr)
        this->getRChild ()->NodeSubsequentTraversal ();

    cout << "Index:" << this->getIndex () << ";Data:" << this->getData () << endl;
}

template <typename T>
int TreeNode<T>::TreeNodePreorderTraversal () {

    //cout<<"Index:"<<this->getIndex()<<";Data:"<<this->getData()<<endl;

    int res = this->getIndex ();
    if (this->getFirstChild () != nullptr)
        res ^= this->getFirstChild ()->TreeNodePreorderTraversal ();

    if (this->getNextSibling () != nullptr)
        res ^= this->getNextSibling ()->TreeNodePreorderTraversal ();

    return res;
}

template <typename T>
//加入0号根节点后，处理二叉树应该有一定的规律
void TreeNode<T>::ConvertToTree () {

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
        pPNode->ConvertToTree ();

        pPNode = pRNode;
    }

}

template <typename T>
//应该也是递归
void TreeNode<T>::ConvertToBiTree () {
    TreeNode<T>* pFirstChild = this->getFirstChild ();
    TreeNode<T>* pNextSibling = this->getNextSibling ();

    //长子为左孩子，下一个兄弟为右孩子
    this->setLChild (pFirstChild);
    this->setRChild (pNextSibling);
    this->setFirstChild (nullptr);
    this->setNextSibling (nullptr);

    if (pFirstChild != nullptr)
        pFirstChild->ConvertToBiTree ();

    if (pNextSibling != nullptr)
        pNextSibling->ConvertToBiTree ();

}

template <typename T>
class Tree {
public:
    Tree (int size, int index, T data);
    Tree (int size);
    virtual ~Tree ();

    bool IsTreeEmpty (); //树是否为空
    bool IsTreeFull ();  //树的容量是否已满

    //search    
    TreeNode<T>* getBiNodeByIndex (int index);     //通过索引搜索节点
    TreeNode<T>* getTreeNodeByIndex (int index);
    int getLeaves ();                            //获取树的叶子数
    int getHeight ();                            //获取树的高度(包含根节点)
    int getWidth ();                             //获取树的宽度(包含根节点)
    int getNowBiSize ();                           //获取树现在的节点数（包含根节点）
    int getNowTreeSize ();
    int getMaxSize ();                           //获取树的最大节点数

    //add/delete
    bool addLeftNodeByIndex (int newIndex, T data, int searchIndex);       //添加左孩子节点（使用索引）
    bool addRightNodeByIndex (int newIndex, T data, int searchIndex);      //添加右孩子节点（使用索引）
    bool addLeftNodeByNode (int index, T data, TreeNode<T>* pNode);      //添加左孩子节点（使用节点地址）
    bool addRightNodeByNode (int index, T data, TreeNode<T>* pNode);     //添加右孩子节点（使用节点地址）

    bool addLeftSubTreeByIndex (TreeNode<T>* pNode, int serchIndex);   //添加左子树（使用索引）
    bool addRightSubTreeByIndex (TreeNode<T>* pNode, int serchIndex);  //添加右子树（使用索引）

    bool addSubTreeByIndex (TreeNode<T>* pNode, int serchIndex);   //添加子树（使用索引）

    bool addChildNodeByIndex (int newIndex, T data, int searchIndex);
    bool addChildNodeByNode (int index, T data, TreeNode<T>* pNode);

    void addEdge (int a, int b);//在森林的根节点a,b间插入一条边，其中a为b节点的父亲

    virtual bool deleteTreeNodeByIndex (int index);          //删除节点,其孩子节点将成为根节点的孩子节点（使用索引）
    virtual bool deleteTreeNodeByNode (TreeNode<T>* pNode);  //删除节点（使用地址）

    //traversal
    void BiPreorderTraversal ();   //先序遍历
    void TreePreorderTraversal (); //先序遍历

    void InorderTraversal ();    //中序遍历
    void SubsequentTraversal (); //后序遍历

    void showTree ();
    void showBiTree ();

    void ConvertToTree ();
    void ConvertToBiTree ();

protected:
    TreeNode<T>* _root;     //树根节点
    int m_iSize;                //当前树的节点数(不包括根节点)
    int m_iMaxSize;             //树的最大节点数(不包括根节点)
};

template <typename T>
Tree<T>::Tree (int size, int index, T data) {

    _root = new TreeNode<T> (index, data);
    _root->setLChild (nullptr);
    _root->setRChild (nullptr);
    _root->setParenet (nullptr);
    _root->setFirstChild (nullptr);
    _root->setNextSibling (nullptr);
    m_iSize = 1;
    m_iMaxSize = size;
}

template <typename T>
Tree<T>::Tree (int size) {

    _root = new TreeNode<T> (0, 0);
    _root->setLChild (nullptr);
    _root->setRChild (nullptr);
    _root->setParenet (nullptr);
    _root->setFirstChild (nullptr);
    _root->setNextSibling (nullptr);
    m_iSize = 1;
    m_iMaxSize = size;
}


template <typename T>
Tree<T>::~Tree () {

    if (_root != nullptr)
        delete _root;
    _root = nullptr;
}

template <typename T>
bool Tree<T>::IsTreeEmpty () {

    if (m_iSize == 0)
        return true;
    return false;
}

template <typename T>
bool Tree<T>::IsTreeFull () {

    if (m_iSize >= m_iMaxSize)
        return true;
    return false;
}

//search
template <typename T>
TreeNode<T>* Tree<T>::getBiNodeByIndex (int index) {

    if (_root == nullptr)
        return nullptr;

    return _root->BiNodeSearch (index);
}

template <typename T>
TreeNode<T>* Tree<T>::getTreeNodeByIndex (int index) {

    if (_root == nullptr)
        return nullptr;

    return _root->TreeNodeSearch (index);
}

template <typename T>
int Tree<T>::getLeaves () {

    if (_root == nullptr)
        return 0;

    return _root->NodeLeavesCount ();
}

template <typename T>
int Tree<T>::getWidth () {
    if (_root == nullptr)
        return 0;

    int maxWidth = 1;         //save max width
    int parentWidth = 0;      //save this width
    int childrenWidth = 0;    //save next width
    queue<TreeNode<T>*> stdQueue;
    TreeNode<T>* tempNode = _root;
    if (tempNode->getLChild () != nullptr) {
        stdQueue.push (tempNode->getLChild ());
        parentWidth++;
    }
    if (tempNode->getRChild () != nullptr) {
        stdQueue.push (tempNode->getRChild ());
        parentWidth++;
    }

    while (!stdQueue.empty ()) {
        while (parentWidth > 0) {
            tempNode = stdQueue.front ();
            stdQueue.pop ();
            if (tempNode->getLChild () != nullptr) {
                stdQueue.push (tempNode->getLChild ());
                childrenWidth++;
            }
            if (tempNode->getRChild () != nullptr) {
                stdQueue.push (tempNode->getRChild ());
                childrenWidth++;
            }
            parentWidth--;
        }
        parentWidth = childrenWidth;
        if (parentWidth > maxWidth) {
            maxWidth = parentWidth;
        }
        childrenWidth = 0;
    }

    //result =  _root->NodeChildrenNodeWidth(&child);
    return maxWidth;
}


template <typename T>
int Tree<T>::getHeight () {

    if (_root == nullptr)
        return 0;
    return _root->NodeChildrenNodeHeigh ();//including root 
}

template <typename T>
int Tree<T>::getNowBiSize () {

    if (_root == nullptr)
        return 0;

    //return m_iSize;//quickly get Size
    return _root->BiNodeChildrenCount ();//including root
}

template <typename T>
int Tree<T>::getNowTreeSize () {

    if (_root == nullptr)
        return 0;

    //return m_iSize;//quickly get Size
    return _root->TreeNodeChildrenCount ();//including root
}

template <typename T>
int Tree<T>::getMaxSize () {

    return m_iMaxSize;
}

//add
template <typename T>
bool Tree<T>::addLeftNodeByIndex (int newIndex, T data, int searchIndex) {

    if (_root == nullptr)
        return false;

    TreeNode<T>* tempNode;
    tempNode = _root->BiNodeSearch (searchIndex);//通过索引找到该节点

    if (tempNode != nullptr)
        return addLeftNodeByNode (newIndex, data, tempNode);

    return false;
}

template <typename T>
bool Tree<T>::addRightNodeByIndex (int newIndex, T data, int searchIndex) {

    if (_root == nullptr)
        return false;

    TreeNode<T>* tempNode;
    tempNode = _root->BiNodeSearch (searchIndex);

    if (tempNode != nullptr)
        return addRightNodeByNode (newIndex, data, tempNode);

    return false;
}

template <typename T>
bool Tree<T>::addLeftNodeByNode (int index, T data, TreeNode<T>* pNode) {

    TreeNode<T>* pNodeCopy = pNode;//做pNode的副本，防止pNode的被意外修改

    if (IsTreeFull ())
        return false;

    if (pNodeCopy->getLChild () == nullptr) {

        TreeNode<T>* newNode = new TreeNode<T> (index, data);
        pNodeCopy->setLChild (newNode);
        newNode->setParenet (pNodeCopy);
    } else
        return false;

    m_iSize++;
    return true;
}

//template <typename T>
//bool Tree<T>::addRightNodeByNode (int index, T data, TreeNode<T>* pNode) {
//
//    TreeNode<T>* pNodeCopy = pNode;
//    if (IsTreeFull ())
//        return false;
//
//    if (pNodeCopy->getRChild () == nullptr) {
//
//        TreeNode<T>* newNode = new TreeNode<T> (index, data);
//        pNodeCopy->setRChild (newNode);
//        newNode->setParenet (pNodeCopy);
//    } else
//        return false;
//
//    m_iSize++;
//    return true;
//}

template <typename T>
bool Tree<T>::addLeftSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {

    if (_root == nullptr)
        return false;

    TreeNode<T>* tempNode;
    tempNode = _root->BiNodeSearch (searchIndex);//通过索引找到该节点

    tempNode->setLChild (pNode);
    m_iSize += pNode->BiNodeChildrenCount ();

    return true;
}


template <typename T>
bool Tree<T>::addRightSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {

    if (_root == nullptr)
        return false;

    TreeNode<T>* tempNode;
    tempNode = _root->BiNodeSearch (searchIndex);//通过索引找到该节点

    tempNode->setRChild (pNode);
    //m_iSize += pNode->BiNodeChildrenCount();

    return true;
}

template <typename T>
bool Tree<T>::addSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {

    if (_root == nullptr)
        return false;

    TreeNode<T>* tempNode;
    tempNode = _root->TreeNodeSearch (searchIndex);//通过索引找到该节点

    tempNode->setChild (pNode);
    //m_iSize += pNode->TreeNodeChildrenCount();

    return true;
}

template <typename T>
bool Tree<T>::addChildNodeByIndex (int newIndex, T data, int searchIndex) {

    if (_root == nullptr)
        return false;

    TreeNode<T>* tempNode;
    tempNode = _root->TreeNodeSearch (searchIndex);//通过索引找到该节点

    if (tempNode != nullptr) {
        //cout << tempNode->getIndex() << endl;
        return addChildNodeByNode (newIndex, data, tempNode);
    }


    return false;
}

template <typename T>
bool Tree<T>::addChildNodeByNode (int index, T data, TreeNode<T>* pNode) {

    TreeNode<T>* pNodeCopy = pNode;//做pNode的副本，防止pNode的被意外修改

    if (IsTreeFull ())
        return false;
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
        //cout << pTempNode->getIndex() << endl;
    }

    m_iSize++;
    return true;
}

template <typename T>
void Tree<T>::addEdge (int a, int b) {
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
template <typename T>
bool Tree<T>::deleteTreeNodeByIndex (int index) {
    //cout << "deleteTreeNodeByIndex(int index)" << endl;

    TreeNode<T>* deleteNode = _root->TreeNodeSearch (index);
    //cout << deleteNode->getIndex() << endl;
    if (deleteNode != nullptr) {
        if (deleteNode == _root) {
            cout << "Tree<T>::deleteTreeNodeByIndex():" << index << "是根节点不能删除" << endl;
            return false;
        }
        return deleteTreeNodeByNode (deleteNode);
    }
    return false;
}

template <typename T>
bool Tree<T>::deleteTreeNodeByNode (TreeNode<T>* pNode) {
    //cout << "deleteTreeNodeByNode(TreeNode<T> *pNode)" << endl;

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

        //delete pNode;     
        //cout << pNode->getIndex() << endl;
        return true;
    }

    return false;
}

//traversal

template <typename T>
void Tree<T>::BiPreorderTraversal () {

    if (_root == nullptr)
        return;

    //cout<<"BiPerorderTraversal:"<<endl;  
    cout << _root->getLChild ()->BiNodePreorderTraversal () << endl;

}
//没写好
template <typename T>
void Tree<T>::TreePreorderTraversal () {

    if (_root == nullptr)
        return;

    //cout<<"TreePerorderTraversal:"<<endl;  
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

//template <typename T>
//void Tree<T>::InorderTraversal () {
//
//    if (_root == nullptr)
//        return;
//
//    cout << "InorderTraversal:" << endl;
//    _root->NodeInorderTraversal ();
//}
//template <typename T>
//void Tree<T>::SubsequentTraversal () {
//
//    if (_root == nullptr)
//        return;
//
//    cout << "SubsequentTraversal:" << endl;
//    _root->NodeSubsequentTraversal ();
//}


template <typename T>
void Tree<T>::ConvertToTree () {
    _root->ConvertToTree ();

}

template <typename T>
void Tree<T>::ConvertToBiTree () {
    _root->ConvertToBiTree ();

}

//template <typename T>
//void Tree<T>::showTree () {
//    cout << "showTree" << endl;
//    _root->showTree ();
//}
//
//template <typename T>
//void Tree<T>::showBiTree () {
//    cout << "showBiTree" << endl;
//    _root->showBiTree ();
//}
#pragma warning(disable:4996)
int main () {
    freopen ("in2.txt", "r", stdin);

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
        for (i = 0; i < M; i++)
            cin >> roots[i];

        //A节点有B个孩子
        for (i = 0; i < N; i++) {
            cin >> A >> B;
            for (j = 0; j < B; j++) {
                cin >> node;
                //cout << A << " " << node << endl;
                nodes[A].setChild (&nodes[node]);
            }
        }

        for (i = 0; i < M; i++) {
            //nodes[roots[i]].showTree();
            tree->addSubTreeByIndex (&nodes[roots[i]], 0);
        }
        //tree->showTree();
    }
    //初始化二叉树
    else {
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
        //tree->showBiTree();
    }
    //cout << tree->getNowTreeSize() << endl;
    //tree->TreePreorderTraversal();

    //进行树的操作
    cin >> Q;
    int op, father, a, b;
    for (i = 0; i < Q; i++) {
        cin >> op;
        //为森林中树的结点 father 插入一个孩子结点 node ，若 father 为 -1 , 表示插入的是孤立结点
        if (op == 1) {
            cin >> father >> node;
            if (father == -1) {
                tree->addSubTreeByIndex (&nodes[node], 0);
            } else {
                tree->addSubTreeByIndex (&nodes[node], father);
            }
            //tree->showTree();
        }

        //删除森林中的结点 node , 其中 father 是 node 的父亲结点，father 若为 -1 ，代表删除根结点。
        else if (op == 2) {
            cin >> father >> node;
            //cout << father << " " << node << endl;
            tree->deleteTreeNodeByIndex (node);
            //tree->showTree();
        }

        //在森林中的根结点 a, b 间插入一条边, 其中 a 为 b 结点的父亲
        else if (op == 3) {
            cin >> a >> b;
            tree->addEdge (a, b);
        }

        //森林、二叉树转换
        else if (op == 4) {
            if (K == 0) {
                tree->ConvertToBiTree ();
                K = 1;
            } else {
                tree->ConvertToTree ();
                K = 0;
            }
        }

        //二叉树的 father 结点插入一个孩子 node
        else if (op == 5) {
            cin >> pos >> father >> node;
            if (pos == 0)
                tree->addRightSubTreeByIndex (&nodes[node], father);

            else
                tree->addLeftSubTreeByIndex (&nodes[node], father);
        }

        //显示森林/二叉树
        else {
            if (K == 0)
                tree->TreePreorderTraversal ();

            else
                tree->BiPreorderTraversal ();
        }

    }
    //system ("pause");
    return 0;
}
