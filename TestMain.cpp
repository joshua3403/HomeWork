#include "stdafx.h"
#include "CBinarySearchTree.h"

int main()
{
	BinarySearchTree<int> testBST(10);
	testBST.InsertNode(new Node<int>(55));
	testBST.InsertNode(new Node<int>(12));
	testBST.InsertNode(new Node<int>(35));
	testBST.InsertNode(new Node<int>(11));
	testBST.InsertNode(new Node<int>(23));
	testBST.InsertNode(new Node<int>(66));
	testBST.InsertNode(new Node<int>(235));
	testBST.InsertNode(new Node<int>(176));
	testBST.InsertNode(new Node<int>(1346));
	testBST.InsertNode(new Node<int>(789));
	testBST.InsertNode(new Node<int>(56));
	testBST.InsertNode(new Node<int>(48));
		   
	testBST.InorderPrintTree(testBST.GetRoot());
	testBST.DestroyTree(testBST.GetRoot());


	return 0;
}