#include "stdafx.h"
#include "CBinarySearchTree.h"

int main()
{
	BinarySearchTree<int> testBST(666);
	Node<int>* Root = testBST.GetRoot();
		   
	testBST.InsertNode(Root, new Node<int>(1346));
	testBST.InsertNode(Root, new Node<int>(22));
	testBST.InsertNode(Root, new Node<int>(346));
	testBST.InsertNode(Root, new Node<int>(11));
	testBST.InsertNode(Root, new Node<int>(156168));
	testBST.InsertNode(Root, new Node<int>(888));
	testBST.InsertNode(Root, new Node<int>(15));
	testBST.InsertNode(Root, new Node<int>(96));

	wprintf(L"Using Size : %d\n", testBST.GetUsingSize());

	testBST.InorderPrintTree(testBST.GetRoot());
	wprintf(L"\n");
	testBST.DestroyTree(testBST.GetRoot());
	wprintf(L"Using Size : %d\n", testBST.GetUsingSize());


	return 0;
}