#pragma once
#include "stdafx.h"

enum COLOR { RED, BLACK };

template<typename T>
class CRedBlackTree;

template<typename T>
class Node
{
	friend class CRedBlackTree<T>;
public:
	Node* m_pParent;
	Node* m_pRight;
	Node* m_pLeft;
	COLOR m_eColor;
	T m_tData;
public:
	Node(T data, Node* parent = nullptr, Node* right = CRedBlackTree<T>::_NIL, Node* left = CRedBlackTree<T>::_NIL)
	{
		m_pParent = parent;
		m_pRight = right;
		m_pLeft = left;
		m_tData = data;
		m_eColor = COLOR::RED;
	}
	Node(COLOR color, Node* parent = nullptr, Node* right = CRedBlackTree<T>::_NIL, Node* left = CRedBlackTree<T>::_NIL)
	{
		m_pParent = parent;
		m_pRight = right;
		m_pLeft = left;
		m_tData = NULL;
		m_eColor = color;
	}
	~Node()
	{
		printf("%d Node deleted\n", m_tData);
	}

	void SetData(T data)
	{
		m_tData = data;
	}
};

template<typename T>
class CRedBlackTree
{
private:
	Node<T>* m_pRoot;
	int m_iNodeCount;

public:
	static Node<T>* _NIL;

public:
	CRedBlackTree()
	{
		m_pRoot = nullptr;
		m_iNodeCount = 0;
	}

	~CRedBlackTree()
	{
		if (m_iNodeCount != 0)
			DestroyTree(m_pRoot);

		delete _NIL;
	}

	Node<T>* GetRoot()
	{
		return m_pRoot;
	}

	void InsertNode(T data)
	{
		if (m_pRoot == nullptr)
		{
			m_pRoot = new Node<T>(data);
			m_pRoot->m_eColor = COLOR::BLACK;
			printf("%d was inserted\n", data);
		}
		else
		{
			if (SearchNode(m_pRoot,data) == nullptr)
			{
				Node<T>* temp = GetRoot();
				Node<T>* newNode = new Node<T>(data);
				bool isLeft = false;

				while (temp != _NIL)
				{
					if (temp->m_tData > data)
					{
						if (temp->m_pLeft == _NIL)
						{
							temp->m_pLeft = newNode;
							printf("%d was inserted\n", data);
							newNode->m_pParent = temp;
							isLeft = true;
							m_iNodeCount++;
							break;
						}
						else
						{
							temp = temp->m_pLeft;
						}

					}
					else
					{
						if (temp->m_pRight == _NIL)
						{
							temp->m_pRight = newNode;
							printf("%d was inserted\n", data);
							newNode->m_pParent = temp;
							isLeft = false;
							m_iNodeCount++;
							break;
						}
						else
						{
							temp = temp->m_pRight;
						}
					}
				}
				BalancingAfterInsert(newNode, isLeft);

			}
			
		}
	}

	void LeftRotate(Node<T>* target)
	{
		if (target == _NIL || _NIL == target->m_pRight)
			return;

		// 새로 부모 노드가 될 타겟 노드의 오른 자식 노드
		Node<T>* temp = target->m_pRight;
		// 타겟 노드의 오른 자식으로 타겟 노드의 오른 자식의 왼자식을 붙여준다
		target->m_pRight = target->m_pRight->m_pLeft;
		if (target->m_pRight != _NIL)
		{
			target->m_pRight->m_pParent = target;
		}
		// 새로 부모노드의 왼자식으로 타겟 노드를 붙여준다
		temp->m_pLeft = target;
		// 부모노드가 된 새 노드의 부모로 타겟 노드의 부모 노드를 붙여준다
		temp->m_pParent = target->m_pParent;
		if (target->m_pParent != nullptr)
		{
			if (target->m_pParent->m_pLeft == target)
				target->m_pParent->m_pLeft = temp;
			else
				target->m_pParent->m_pRight = temp;
		}

		target->m_pParent = temp;
		if (m_pRoot == target)
			m_pRoot = temp;
	}


	void RightRotate(Node<T>* target)
	{
		if (target == _NIL || _NIL == target->m_pLeft)
			return;

		// 새로 부모 노드가 될 타겟 노드의 왼쪽 자식 노드
		Node<T>* temp = target->m_pLeft;
		// 타겟 노드의 오른 자식으로 타겟 노드의 왼 자식의 오른 자식을 붙여준다
		target->m_pLeft = target->m_pLeft->m_pRight;
		if (target->m_pLeft != _NIL)
		{
			target->m_pLeft->m_pParent = target;
		}
		// 새로 부모노드의 오른 자식으로 타겟 노드를 붙여준다
		temp->m_pRight = target;
		// 부모노드가 된 새 노드의 부모로 타겟 노드의 부모 노드를 붙여준다
		temp->m_pParent = target->m_pParent;
		if (target->m_pParent != nullptr)
		{
			if (target->m_pParent->m_pLeft == target)
				target->m_pParent->m_pLeft = temp;
			else
				target->m_pParent->m_pRight = temp;
		}

		target->m_pParent = temp;
		if (m_pRoot == target)
			m_pRoot = temp;
	}

	void BalancingAfterInsert(Node<T>* target, bool left)
	{
		Node<T>* parent = target->m_pParent;
		while (nullptr != parent && RED == parent->m_eColor)
		{
			// in loop grand parent must exist
			Node<T>* grandParent = parent->m_pParent;
			bool leftParent = grandParent->m_pLeft == parent;
			Node<T>* uncle = leftParent ? grandParent->m_pRight : grandParent->m_pLeft;
			if (RED == uncle->m_eColor)
			{
				parent->m_eColor = COLOR::BLACK;
				uncle->m_eColor = COLOR::BLACK;
				grandParent->m_eColor = COLOR::RED;
				target = grandParent;
				parent = grandParent->m_pParent;
				if (nullptr != parent)
					left = parent->m_pLeft == target;
			}
			else
			{
				if ((leftParent && false == left) || (false == leftParent && left))
				{
					leftParent ? LeftRotate(parent) : RightRotate(parent);
					parent = target;
					left = !left;
				}

				parent->m_eColor = COLOR::BLACK;
				grandParent->m_eColor = RED;
				left ? RightRotate(grandParent) : LeftRotate(grandParent);
				break;
			}
		}
		m_pRoot->m_eColor = COLOR::BLACK;
	}

	Node<T>* SearchNode(Node<T>* currentNode, T data)
	{
		if (currentNode == _NIL)
			return nullptr;

		if (currentNode->m_tData == data)
			return currentNode;
		else if (currentNode->m_tData > data)
			return SearchNode(currentNode->m_pLeft, data);
		else
			return SearchNode(currentNode->m_pRight, data);
	}

	void InorderTraversal(Node<T>* temp)
	{
		if (temp == _NIL) { return; }
		InorderTraversal(temp->m_pLeft);

		if (temp->m_eColor == COLOR::BLACK)
			printf("%d, BLACK\n", temp->m_tData);
		else
			printf("%d, RED\n", temp->m_tData);

		InorderTraversal(temp->m_pRight);
	}

	void DestroyTree(Node<T>* Tree)
	{
		if (Tree->m_pRight != _NIL)
			DestroyTree(Tree->m_pRight);

		if (Tree->m_pLeft != _NIL)
			DestroyTree(Tree->m_pLeft);

		Tree->m_pLeft = nullptr;
		Tree->m_pRight = nullptr;
		m_iNodeCount--;
		delete Tree;
	}


	void DestroyNode(Node<T>* Node)
	{
		m_iNodeCount--;
		delete Node;
	}

	Node<T>* SearchMinNode(Node<T>* Tree)
	{
		Node<T>* SearchNode = Tree;
		while (SearchNode->m_pLeft != _NIL)
		{
			SearchNode = SearchNode->m_pLeft;
		}
		return SearchNode;
	}

	Node<T>* DeleteNode(Node<T>* Tree, Node<T>* Parent, int data)
	{
		if (Tree == _NIL)
			return nullptr;

		if (SearchNode(Tree, data) == nullptr)
			return nullptr;

		Node<T>* RemoveNode = nullptr;

		if (Tree->m_tData < data)
		{
			DeleteNode(Tree->m_pRight, Tree, data);
		}
		else if (Tree->m_tData > data)
		{
			DeleteNode(Tree->m_pLeft, Tree, data);
		}
		else
		{
			RemoveNode = Tree;

			if (Tree->m_pLeft == _NIL && Tree->m_pRight == _NIL)
			{
				if (Parent->m_pLeft == Tree)
				{
					Parent->m_pLeft = _NIL;

				}
				else
				{
					Parent->m_pRight = _NIL;

				}
			}
			else
			{
				if (Tree->m_pLeft != _NIL && Tree->m_pRight != _NIL)
				{
					T temp = Tree->m_tData;
					Node<T>* minNode = SearchMinNode(Tree->m_pRight);
					Tree->m_tData = minNode->m_tData;
					minNode->SetData(temp);
					//minNode = DeleteNode(Tree, _NIL, minNode->m_tData);
				}
				else
				{
					Node<T>* temp = nullptr;
					if (Tree->m_pLeft != _NIL)
					{
						temp = Tree->m_pLeft;
					}
					else
					{
						temp = Tree->m_pRight;
					}

					if (Parent->m_pLeft == Tree)
						Parent->m_pLeft = temp;
					else
						Parent->m_pRight = temp;
				}
			}
		}

		return RemoveNode;
	}


};

template<typename T>
Node<T>* CRedBlackTree<T>::_NIL = new Node<T>(COLOR::BLACK);