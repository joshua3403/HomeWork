#include "stdafx.h"
#include "CRedBlackTree.h"

int main()
{
	CRedBlackTree<int> test;

	test.InsertNode(123);

	test.InsertNode(11);
	test.InsertNode(23);
	test.InsertNode(457);
	test.InsertNode(334);

	test.InsertNode(66);
	test.InsertNode(55);



	printf("------------------���� ��ȸ------------------\n");
	test.InorderTraversal(test.GetRoot());
	printf("root = %d\n", test.GetRoot()->m_tData);
	printf("------------------���� ��ȸ ��------------------\n");

	return 0;
}