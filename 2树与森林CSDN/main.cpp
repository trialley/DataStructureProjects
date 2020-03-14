#include "BinaryTree.h"
#include "Tree.h"
//#include "binaryTreeToForest.h"
//#include "ForestToBinaryTree.h"
#include"Forest.h"
#include <iostream>
#include <vector>
using namespace std;


int main () {
	int initElement[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	BTree<int> binaryTree;
	binaryTree.initialize (initElement, 20);
	binaryTree.levelOrderPrint ();
	cout << endl;

	//vector<unique_ptr<Tree<int> > > pTreeVector = binaryTreeToForest<int> (binaryTree);
	//for (size_t i = 0; i < pTreeVector.size (); ++i) {
	//	pTreeVector.at (i)->levelOrderPrint ();
	//	cout << "..............................." << endl;
	//}

	//vector<Tree<int>*> pForestVector;
	//for (int i = 0; i < pTreeVector.size (); ++i) {
	//	TreeNode<int>* pTreeNode = pTreeVector.at (i)->root ();
	//	Tree<int>* pTree = new Tree<int> (pTreeNode);
	//	pForestVector.push_back (pTree);
	//}

	//unique_ptr<BTree<int> >pNode = forestToBinaryTree<int> (pForestVector);
	//pNode->levelOrderPrint ();

	//cout << endl;




	Forest<int> f (binaryTree);

	BTree<int> t (f);
	t.levelOrderPrint ();
	return 0;
}
