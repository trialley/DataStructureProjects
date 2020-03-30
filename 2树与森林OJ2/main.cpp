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

    TreeNode* getInorderPrecursor ();      //��ȡ����ǰ��
    TreeNode* getInorderSubsequence ();    //��ȡ������

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
    TreeNode* BiNodeSearch (int index);      //ͨ�����������ڵ㣨�Ա��ڵ���Ϊ��Ѱ������ĳ���ڵ㣩
    TreeNode* TreeNodeSearch (int index);
    int NodeLeavesCount (int leaves = 0);    //ͳ��Ҷ����
    int NodeChildrenNodeHeigh ();            //ͳ���Ա��ڵ���Ϊ�������ĸ߶�
    int BiNodeChildrenCount ();              //ͳ�ƶ������ӽڵ���(�������ڵ�)
    int TreeNodeChildrenCount ();
    int BTreeDeleteAll ();                       //ɾ���������ڵ�
    int TreeNodeDelete ();

    //traversal
    int BiNodePreorderTraversal ();         //������ǰ�����
    int TreeNodePreorderTraversal ();

    void NodeInorderTraversal ();            //�������������
    void NodeSubsequentTraversal ();         //�������������

    void showTree ();
    void showBiTree ();

    void ConvertToTree ();
    void ConvertToBiTree ();

private:
    int _index;               //����
    T _data;                  //ֵ

    TreeNode* _parent;        //���׽ڵ�
    TreeNode* _leftChild;     //���ӽڵ�
    TreeNode* _rightChild;    //�Һ��ӽڵ�
    TreeNode* _firstChild;     //���ӽڵ�
    TreeNode* _nextSibling;    //�ֵܽڵ�

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

//��ȡ����ǰ��
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

//��ȡ������
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
    //�޺��ӵ����
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
        }
        //�ýڵ��������ڵ�֮��
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

/*��ѯ�����ĸ߶�(�����Լ�)*/
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

/*��ѯ������Ҷ����*/
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

/*��ѯ���������Ľڵ���(�����Լ�)*/
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

/*��ѯ�����Ľڵ���(�����Լ�)*/
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

/*ɾ���������������нڵ�*/
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

/*ɾ�����������нڵ�*/
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
//����0�Ÿ��ڵ�󣬴��������Ӧ����һ���Ĺ���
void TreeNode<T>::ConvertToTree () {

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
        pPNode->ConvertToTree ();

        pPNode = pRNode;
    }

}

template <typename T>
//Ӧ��Ҳ�ǵݹ�
void TreeNode<T>::ConvertToBiTree () {
    TreeNode<T>* pFirstChild = this->getFirstChild ();
    TreeNode<T>* pNextSibling = this->getNextSibling ();

    //����Ϊ���ӣ���һ���ֵ�Ϊ�Һ���
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

    bool IsTreeEmpty (); //���Ƿ�Ϊ��
    bool IsTreeFull ();  //���������Ƿ�����

    //search    
    TreeNode<T>* getBiNodeByIndex (int index);     //ͨ�����������ڵ�
    TreeNode<T>* getTreeNodeByIndex (int index);
    int getLeaves ();                            //��ȡ����Ҷ����
    int getHeight ();                            //��ȡ���ĸ߶�(�������ڵ�)
    int getWidth ();                             //��ȡ���Ŀ��(�������ڵ�)
    int getNowBiSize ();                           //��ȡ�����ڵĽڵ������������ڵ㣩
    int getNowTreeSize ();
    int getMaxSize ();                           //��ȡ�������ڵ���

    //add/delete
    bool addLeftNodeByIndex (int newIndex, T data, int searchIndex);       //������ӽڵ㣨ʹ��������
    bool addRightNodeByIndex (int newIndex, T data, int searchIndex);      //����Һ��ӽڵ㣨ʹ��������
    bool addLeftNodeByNode (int index, T data, TreeNode<T>* pNode);      //������ӽڵ㣨ʹ�ýڵ��ַ��
    bool addRightNodeByNode (int index, T data, TreeNode<T>* pNode);     //����Һ��ӽڵ㣨ʹ�ýڵ��ַ��

    bool addLeftSubTreeByIndex (TreeNode<T>* pNode, int serchIndex);   //�����������ʹ��������
    bool addRightSubTreeByIndex (TreeNode<T>* pNode, int serchIndex);  //�����������ʹ��������

    bool addSubTreeByIndex (TreeNode<T>* pNode, int serchIndex);   //���������ʹ��������

    bool addChildNodeByIndex (int newIndex, T data, int searchIndex);
    bool addChildNodeByNode (int index, T data, TreeNode<T>* pNode);

    void addEdge (int a, int b);//��ɭ�ֵĸ��ڵ�a,b�����һ���ߣ�����aΪb�ڵ�ĸ���

    virtual bool deleteTreeNodeByIndex (int index);          //ɾ���ڵ�,�亢�ӽڵ㽫��Ϊ���ڵ�ĺ��ӽڵ㣨ʹ��������
    virtual bool deleteTreeNodeByNode (TreeNode<T>* pNode);  //ɾ���ڵ㣨ʹ�õ�ַ��

    //traversal
    void BiPreorderTraversal ();   //�������
    void TreePreorderTraversal (); //�������

    void InorderTraversal ();    //�������
    void SubsequentTraversal (); //�������

    void showTree ();
    void showBiTree ();

    void ConvertToTree ();
    void ConvertToBiTree ();

protected:
    TreeNode<T>* _root;     //�����ڵ�
    int m_iSize;                //��ǰ���Ľڵ���(���������ڵ�)
    int m_iMaxSize;             //�������ڵ���(���������ڵ�)
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
    tempNode = _root->BiNodeSearch (searchIndex);//ͨ�������ҵ��ýڵ�

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

    TreeNode<T>* pNodeCopy = pNode;//��pNode�ĸ�������ֹpNode�ı������޸�

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
    tempNode = _root->BiNodeSearch (searchIndex);//ͨ�������ҵ��ýڵ�

    tempNode->setLChild (pNode);
    m_iSize += pNode->BiNodeChildrenCount ();

    return true;
}


template <typename T>
bool Tree<T>::addRightSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {

    if (_root == nullptr)
        return false;

    TreeNode<T>* tempNode;
    tempNode = _root->BiNodeSearch (searchIndex);//ͨ�������ҵ��ýڵ�

    tempNode->setRChild (pNode);
    //m_iSize += pNode->BiNodeChildrenCount();

    return true;
}

template <typename T>
bool Tree<T>::addSubTreeByIndex (TreeNode<T>* pNode, int searchIndex) {

    if (_root == nullptr)
        return false;

    TreeNode<T>* tempNode;
    tempNode = _root->TreeNodeSearch (searchIndex);//ͨ�������ҵ��ýڵ�

    tempNode->setChild (pNode);
    //m_iSize += pNode->TreeNodeChildrenCount();

    return true;
}

template <typename T>
bool Tree<T>::addChildNodeByIndex (int newIndex, T data, int searchIndex) {

    if (_root == nullptr)
        return false;

    TreeNode<T>* tempNode;
    tempNode = _root->TreeNodeSearch (searchIndex);//ͨ�������ҵ��ýڵ�

    if (tempNode != nullptr) {
        //cout << tempNode->getIndex() << endl;
        return addChildNodeByNode (newIndex, data, tempNode);
    }


    return false;
}

template <typename T>
bool Tree<T>::addChildNodeByNode (int index, T data, TreeNode<T>* pNode) {

    TreeNode<T>* pNodeCopy = pNode;//��pNode�ĸ�������ֹpNode�ı������޸�

    if (IsTreeFull ())
        return false;
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
template <typename T>
bool Tree<T>::deleteTreeNodeByIndex (int index) {
    //cout << "deleteTreeNodeByIndex(int index)" << endl;

    TreeNode<T>* deleteNode = _root->TreeNodeSearch (index);
    //cout << deleteNode->getIndex() << endl;
    if (deleteNode != nullptr) {
        if (deleteNode == _root) {
            cout << "Tree<T>::deleteTreeNodeByIndex():" << index << "�Ǹ��ڵ㲻��ɾ��" << endl;
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
//ûд��
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
        for (i = 0; i < M; i++)
            cin >> roots[i];

        //A�ڵ���B������
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
    //��ʼ��������
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

    //�������Ĳ���
    cin >> Q;
    int op, father, a, b;
    for (i = 0; i < Q; i++) {
        cin >> op;
        //Ϊɭ�������Ľ�� father ����һ�����ӽ�� node ���� father Ϊ -1 , ��ʾ������ǹ������
        if (op == 1) {
            cin >> father >> node;
            if (father == -1) {
                tree->addSubTreeByIndex (&nodes[node], 0);
            } else {
                tree->addSubTreeByIndex (&nodes[node], father);
            }
            //tree->showTree();
        }

        //ɾ��ɭ���еĽ�� node , ���� father �� node �ĸ��׽�㣬father ��Ϊ -1 ������ɾ������㡣
        else if (op == 2) {
            cin >> father >> node;
            //cout << father << " " << node << endl;
            tree->deleteTreeNodeByIndex (node);
            //tree->showTree();
        }

        //��ɭ���еĸ���� a, b �����һ����, ���� a Ϊ b ���ĸ���
        else if (op == 3) {
            cin >> a >> b;
            tree->addEdge (a, b);
        }

        //ɭ�֡�������ת��
        else if (op == 4) {
            if (K == 0) {
                tree->ConvertToBiTree ();
                K = 1;
            } else {
                tree->ConvertToTree ();
                K = 0;
            }
        }

        //�������� father ������һ������ node
        else if (op == 5) {
            cin >> pos >> father >> node;
            if (pos == 0)
                tree->addRightSubTreeByIndex (&nodes[node], father);

            else
                tree->addLeftSubTreeByIndex (&nodes[node], father);
        }

        //��ʾɭ��/������
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
