#include "stdafx.h"

enum COLOR { RED, BLACK };

template<typename T>
class Node
{
	friend class CRedBlackTree<T>;
private:
	Node* m_pParent;
	Node* m_pRight;
	Node* m_pLeft;
	COLOR m_eColor;
	T m_tData;
public:
	Node(T data, Node* parent = nullptr, Node* right = nullptr, Node* left = nullptr)
	{
		m_pParent = parent;
		m_pRight = right;
		m_pLeft = left;
		m_tData = data;
		m_eColor = COLOR::RED;
	}
	Node(COLOR color, Node* parent = nullptr, Node* right = nullptr, Node* left = nullptr)
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
};

template<typename T>
class CRedBlackTree
{
private:
	Node<T>* m_pRoot;
	static Node<T>* _NIL;
	int m_iNodeCount;
public:
	CRedBlackTree(T data)
	{
		m_pRoot = new Node<T>(data);
		m_iNodeCount = 0;
	}

	~CRedBlackTree()
	{
		if (m_iNodeCount != 0)
			DestroyTree(m_pRoot);
	}

	Node<T>* CreateNewNode(T data)
	{
		Node<T>* newNode = new Node<T>(data);
		newNode->m_pRight = _NIL;
		newNode->m_pRight = _NIL;

		return newNode;
	}

	void RotateLeft(Node<T>* target)
	{
		if (target == _NIL || target->m_pRight == _NIL)
			return;

		// ���� �θ��尡 �� ���
		Node<T>* temp = target->m_pRight;
		// Ÿ�� ����� ������ �ڽ��� ���� �ڽ��� Ÿ�� ����� ������ �ڽ����� ����
		target->m_pRight = target->m_pRight->m_pLeft;
		if (_NIL != target->m_pRight)
		{
			// Ÿ�� ����� ������ �ڽ��� ���� �ڽ��� Ÿ�� ����� ������ �ڽ����� ������ ��
			// �θ� Ÿ������ ����
			target->m_pRight->m_pParent = target;
		}
		// �� �θ� ����� ���� �ڽ��� Ÿ�� ���
		temp->m_pLeft = target;
		// �� �θ� ����� �θ� ���� Ÿ�� ����� �θ� ���
		temp->m_pParent = target->m_pParent;

		// Ÿ�� ����� �θ��尡 �����ϴ� ���(�ƴ� ��� Ÿ�� ���� ��Ʈ ���)
		if (target->m_pParent != nullptr)
		{
			// Ÿ�� ��尡 Ÿ�� ����� �θ� ����� ���� �ڽ��� ���
			if (target->m_pParent->m_pLeft == target)
				target->m_pParent->m_pLeft = temp;
			else
				target->m_pParent->m_pRight = temp;
		}

		target->m_pParent = temp;
		if (m_pRoot == target)
			m_pRoot = target;
	}

	void RotateRight(Node<T>* target)
	{
		if (target == _NIL || target->m_pLeft == _NIL)
			return;

		// ���� �θ� �� ���
		Node<T>* temp = target->m_pLeft;
		// Ÿ�� ����� ���� ����� ������ �ڽ� ��带 Ÿ�� ����� ���� �ڽ����� ����
		target->m_pLeft = target->m_pLeft->m_pRight;
		if (target->m_pLeft != _NIL)
		{
			// Ÿ�� ����� �θ��� ���� �ڽ��� ������ �ڽ��� Ÿ�� ����� ���� �ڽ����� ���� ��
			// �θ� ���� ����
			target->m_pLeft->m_pParent = target;
		}
		// �� �θ� ����� ������ �ڽ��� Ÿ�� ���
		temp->m_pRight = target;
		// �� �θ� ����� �θ� ���� Ÿ�� ����� �θ� ���
		temp->m_pParent = target->m_pParent;

		// Ÿ�� ����� �θ��尡 �����ϴ� ���(�ƴ� ��� Ÿ�� ���� ��Ʈ ���)
		if (target->m_pParent != nullptr)
		{
			// Ÿ�� ��尡 Ÿ�� ����� �θ� ����� ���� �ڽ��� ���
			if (target->m_pParent->m_pLeft == target)
				target->m_pParent->m_pLeft = temp;
			else
				target->m_pParent->m_pRight = temp;
		}
		target->m_pParent = temp;
		if (m_pRoot == target)
			m_pRoot = target;
	}

	Node<T>* SearchNode(Node<T>* currentNode, T data)
	{
		if (currentNode == nullptr)
			return nullptr;

		if (currentNode->m_tData == data)
			return currentNode;
		else if (currentNode->m_tData > data)
			SearchNode(currentNode->m_pLeft, data);
		else
			SearchNode(currentNode->m_pRight, data);
	}

	void InsertNode(Node<T>* Root, Node<T>* newNode)
	{

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
};

template<typename T>
Node<T>* CRedBlackTree<T>::_NIL = new Node<T>(COLOR::BLACK);