#pragma once

#include "stdafx.h"

template<typename T>
class BinarySearchTree;

template<typename T>
class Node
{
	friend class BinarySearchTree<T>;
private:
	T m_tData;
	Node* m_pLeft;
	Node* m_pRight;
public:
	Node(T data = 0, Node* left = nullptr, Node* right = nullptr)
	{
		m_tData = data;
		m_pLeft = left;
		m_pRight = right;
	}
	~Node()
	{
		wprintf(L"Delete Node %d\n", m_tData);
	}
};

template<typename T>
class BinarySearchTree
{
private:
	Node<T>* m_pRoot;
	int m_iUsingCount;
public:
	BinarySearchTree(T data)
	{
		m_pRoot = new Node<T>(data);
		m_iUsingCount = 1;
	}

	~BinarySearchTree()
	{
		if (m_iUsingCount != 0)
			DestroyTree(m_pRoot);
	}

	void InsertNode(Node<T>* Root,Node<T>* newNode)
	{
		// 중복을 먼저 확인
		if (SearchNode(Root, newNode->m_tData) == nullptr)
		{
			if (Root->m_tData < newNode->m_tData)
			{
				if (Root->m_pRight == nullptr)
				{
					Root->m_pRight = newNode;
					wprintf(L"InsertNode : %d, Right\n", newNode->m_tData);
					m_iUsingCount++;
				}
				else
					InsertNode(Root->m_pRight, newNode);

			}
			else if (Root->m_tData > newNode->m_tData)
			{
				if (Root->m_pLeft == nullptr)
				{
					Root->m_pLeft = newNode;
					wprintf(L"InsertNode : %d, Left\n", newNode->m_tData);
					m_iUsingCount++;
				}
				else
					InsertNode(Root->m_pLeft, newNode);

			}

		}
	}
	
	int GetUsingSize(void)
	{
		return m_iUsingCount;
	}

	Node<T>* SearchNode(Node<T>* currentNode, T data)
	{
		if (currentNode == nullptr)
			return nullptr;

		if (currentNode->m_tData == data)
			return currentNode;
		else if (currentNode->m_tData > data)
			return SearchNode(currentNode->m_pLeft, data);
		else
			return SearchNode(currentNode->m_pRight, data);
	}

	Node<T>* GetRoot()
	{
		return m_pRoot;
	}

	void InorderPrintTree(Node<T>* Root)
	{
		if (Root == nullptr)
			return;

		InorderPrintTree(Root->m_pLeft);

		wprintf(L"%d\t", Root->m_tData);

		InorderPrintTree(Root->m_pRight);
	}

	void DestroyTree(Node<T>* Tree)
	{
		if (Tree->m_pRight != nullptr)
			DestroyTree(Tree->m_pRight);

		if (Tree->m_pLeft != nullptr)
			DestroyTree(Tree->m_pLeft);

		Tree->m_pLeft = nullptr;
		Tree->m_pRight = nullptr;
		m_iUsingCount--;
		delete Tree;
	}

	void DestroyNode(Node<T>* Node)
	{
		delete Node;
	}
};
