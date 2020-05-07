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
};

template<typename T>
class BinarySearchTree
{
private:
	Node<T>* m_pRoot;
public:
	BinarySearchTree(T data)
	{
		m_pRoot = new Node<T>(data);
	}

	~BinarySearchTree()
	{
		DestroyTree(m_pRoot);
	}

	void InsertNode(Node<T>* newNode)
	{
		// 중복을 먼저 확인
		if (SearchNode(m_pRoot, newNode->m_tData) == nullptr)
		{
			Node<T>* Parent = nullptr;
			Node<T>* Current = nullptr;
			while (Current != nullptr)
			{
				Parent = Current;
				if (Parent->m_tData > newNode->m_tData)
				{
					Current = Current->m_pLeft;
				}
				else
					Current = Current->m_pRight;
			}
			if (newNode->m_tData < m_pRoot->m_tData)
				Parent->m_pLeft = newNode;
			else
				Parent->m_pRight = newNode;

			wprintf(L"%d was inserted", newNode->m_tData);
		}
	}

	Node<T>* SearchNode(Node<T>* currentNode, T data)
	{
		if (currentNode == nullptr)
			return nullptr;

		if (currentNode->m_tData == data)
			return currentNode;
		else
		{
			if (currentNode->m_tData > data)
				SearchNode(currentNode->m_pLeft, data);
			else
				SearchNode(currentNode->m_pRight, data);
		}
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

		wprintf(L"%d", Root->m_tData);

		InorderPrintTree(Root->m_pRight);
	}

	void DestroyTree(Node<T>* Tree)
	{
		if (Tree->m_pLeft != nullptr)
			DestroyTree(Tree->m_pLeft);

		if (Tree->m_pRight != nullptr)
			DestroyTree(Tree->m_pRight);

		Tree->m_pLeft = nullptr;
		Tree->m_pRight = nullptr;

		wprintf(L"%d was deleted", Tree->m_tData);

		free( Tree);
	}
};
