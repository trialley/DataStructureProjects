/*
@Author: TriAlley
@Date: 2020/02/25
@refrence:
    1. Skip Lists : A Probabilistic Alternative to Balanced Trees
    2. https://www.cnblogs.com/gqtcgq/p/7247074.html
*/
#pragma once
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
using namespace std;




template<class K, class E>
class TApair {
public:
    K first;
    E second;
    TApair ( TApair& i) :first(i.first),second(i.second){

    }
    TApair (K& k,E& e) :first (k), second (e) {

    }
    TApair (){

    }
};


template<class K, class E>
class dictionary {
public:
    virtual ~dictionary () {}
    virtual bool empty ()  = 0;//返回是否为空
    virtual int size ()  = 0;//返回元素数目
    virtual TApair< K, E>* find ( K&)  = 0;//根据关键词找到键值对
    virtual void erase ( K&) = 0;//根据关键词删除键值对
    virtual void insert ( TApair< K, E>&) = 0;//插入键值对
};

template <class K, class E>
struct skipNode {
    typedef TApair<K, E> TApairType;
    TApairType element;
    skipNode<K, E>** next;   // 1D array of pointers

    skipNode ( TApairType& theTApair, int size):element (theTApair) {
        next = new skipNode<K, E> * [size];
    }
};

template<class K, class E>
class skipList : public dictionary<K, E> {
public:
    skipList (K, int maxTApairs = 10000, float prob = 0.5);
    ~skipList ();

    bool empty ()  { return dSize == 0; }
    int size ()  { return dSize; }
    TApair< K, E>* find (K&);
    TApair< K, E>* getMin () {
        return &headerNode->next[0]->element;
    }
    TApair< K, E>* getMax () {
        return &tailNode->element;
    }
    void erase ( K&);
    void insert ( TApair< K, E>&);
    void output (ostream& out) ;

protected:
    float cutOff;          // used to decide level number
    int randLevel () ;     // generate a random level number
    int levels;            // max current nonempty chain
    int dSize;             // number of TApairs in dictionary
    int maxLevel;          // max permissible chain level
    K tailKey;             // a large key
    skipNode<K, E>* search ( K&) ;
    // search saving last nodes seen
    skipNode<K, E>* headerNode;  // header node pointer
    skipNode<K, E>* tailNode;    // tail node pointer
    skipNode<K, E>** last;       // last[i] = last node seen on level i
};

template<class K, class E>
skipList<K, E>::skipList (K largeKey, int maxTApairs, float prob) {// ructor for skip lists with keys smaller than largeKey and
 // size at most maxTApairs. 0 < prob < 1.
    cutOff = prob * RAND_MAX;
    maxLevel = (int)ceil (logf ((float)maxTApairs) / logf (1 / prob)) - 1;
    levels = 0;  // initial number of levels
    dSize = 0;
    tailKey = largeKey;

    // create header & tail nodes and last array
    TApair<K, E> tailTApair;
    tailTApair.first = tailKey;
    headerNode = new skipNode<K, E> (tailTApair, maxLevel + 1);
    tailNode = new skipNode<K, E> (tailTApair, 0);
    last = new skipNode<K, E> * [maxLevel + 1];

    // header points to tail at all levels as lists are empty
    for (int i = 0; i <= maxLevel; i++)
        headerNode->next[i] = tailNode;
}

template<class K, class E>
skipList<K, E>::~skipList () {// Delete all nodes and array last.
    skipNode<K, E>* nextNode;

    // delete all nodes by following level 0 chain
    while (headerNode != tailNode) {
        nextNode = headerNode->next[0];
        delete headerNode;
        headerNode = nextNode;
    }
    delete tailNode;

    delete[] last;
}

template<class K, class E>
TApair< K, E>* skipList<K, E>::find ( K& theKey)  {// Return pointer to matching TApair.
 // Return NULL if no matching TApair.
    if (theKey >= tailKey)
        return NULL;  // no matching TApair possible

     // position beforeNode just before possible node with theKey
    skipNode<K, E>* beforeNode = headerNode;
    for (int i = levels; i >= 0; i--)          // go down levels
       // follow level i pointers
        while (beforeNode->next[i]->element.first < theKey)
            beforeNode = beforeNode->next[i];

    // check if next node has theKey
    if (beforeNode->next[0]->element.first == theKey)
        return &beforeNode->next[0]->element;

    return NULL;  // no matching TApair
}

template<class K, class E>
int skipList<K, E>::randLevel ()  {// Return a random level number <= maxLevel.
    int lev = 0;
    while (rand () <= cutOff)
        lev++;
    return (lev <= maxLevel) ? lev : maxLevel;
}

template<class K, class E>
skipNode<K, E>* skipList<K, E>::search ( K& theKey)  {// Search for theKey saving last nodes seen at each
 // level in the array last
 // Return node that might contain theKey.
   // position beforeNode just before possible node with theKey
    skipNode<K, E>* beforeNode = headerNode;
    for (int i = levels; i >= 0; i--) {
        while (beforeNode->next[i]->element.first < theKey)
            beforeNode = beforeNode->next[i];
        last[i] = beforeNode;  // last level i node seen
    }
    return beforeNode->next[0];
}

template<class K, class E>
void skipList<K, E>::insert ( TApair< K, E>& theTApair) {
    if (theTApair.first >= tailKey){//关键词非法
        ostringstream s;
        s << "Key = " << theTApair.first << " Must be < " << tailKey;
        throw s.str ();
    }

    // see if TApair with theKey already present
    skipNode<K, E>* theNode = search (theTApair.first);
    if (theNode->element.first == theTApair.first) {// update theNode->element.second
        theNode->element.second = theTApair.second;
        return;
    }

    // not present, determine level for new node
    int theLevel = randLevel (); // level of new node
    // fix theLevel to be <= levels + 1
    if (theLevel > levels) {
        theLevel = ++levels;
        last[theLevel] = headerNode;
    }

    // get and insert new node just after theNode
    skipNode<K, E>* newNode = new skipNode<K, E> (theTApair, theLevel + 1);
    for (int i = 0; i <= theLevel; i++) {// insert into level i chain
        newNode->next[i] = last[i]->next[i];
        last[i]->next[i] = newNode;
    }

    dSize++;
    return;
}

template<class K, class E>
void skipList<K, E>::erase ( K& theKey) {// Delete the TApair, if any, whose key equals theKey.
    if (theKey >= tailKey) // too large
        return;

    // see if matching TApair present
    skipNode<K, E>* theNode = search (theKey);
    if (theNode->element.first != theKey) // not present
        return;

    // delete node from skip list
    for (int i = 0; i <= levels &&
        last[i]->next[i] == theNode; i++)
        last[i]->next[i] = theNode->next[i];

    // update levels
    while (levels > 0 && headerNode->next[levels] == tailNode)
        levels--;

    delete theNode;
    dSize--;
}

template<class K, class E>
void skipList<K, E>::output (ostream& out)  {// Insert the dictionary TApairs into the stream out.
   // follow level 0 chain
    for (skipNode<K, E>* currentNode = headerNode->next[0];
        currentNode != tailNode;
        currentNode = currentNode->next[0])
        out 
        << currentNode->element.first << ":"
        << currentNode->element.second << "  ";
}

// overload <<
template <class K, class E>
ostream& operator<<(ostream& out,  skipList<K, E>& x) {
    x.output (out); return out;
}