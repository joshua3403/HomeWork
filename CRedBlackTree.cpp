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

		// 새로 부모노드가 될 노드
		Node<T>* temp = target->m_pRight;
		// 타겟 노드의 오른쪽 자식의 왼쪽 자식을 타겟 노드의 오른쪽 자식으로 연결
		target->m_pRight = target->m_pRight->m_pLeft;
		if (_NIL != target->m_pRight)
		{
			// 타겟 노드의 오른쪽 자식의 왼쪽 자식을 타겟 노드의 오른쪽 자식으로 연결한 후
			// 부모를 타겟으로 설정
			target->m_pRight->m_pParent = target;
		}
		// 새 부모 노드의 왼쪽 자식은 타겟 노드
		temp->m_pLeft = target;
		// 새 부모 노드의 부모 노드는 타켓 노드의 부모 노드
		temp->m_pParent = target->m_pParent;

		// 타겟 노드의 부모노드가 존재하는 경우(아닌 경우 타겟 노드는 루트 노드)
		if (target->m_pParent != nullptr)
		{
			// 타겟 노드가 타겟 노드의 부모 노드의 왼쪽 자식인 경우
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

		// 새로 부모가 될 노드
		Node<T>* temp = target->m_pLeft;
		// 타겟 노드의 왼쪽 노드의 오른쪽 자식 노드를 타겟 노드의 왼쪽 자식으로 연결
		target->m_pLeft = target->m_pLeft->m_pRight;
		if (target->m_pLeft != _NIL)
		{
			// 타겟 노드의 부모의 왼쪽 자식의 오른쪽 자식을 타겟 노드의 왼쪽 자식으로 연결 후
			// 부모 노드로 설정
			target->m_pLeft->m_pParent = target;
		}
		// 새 부모 노드의 오른쪽 자식은 타겟 노드
		temp->m_pRight = target;
		// 새 부모 노드의 부모 노드는 타겟 노드의 부모 노드
		temp->m_pParent = target->m_pParent;

		// 타겟 노드의 부모노드가 존재하는 경우(아닌 경우 타겟 노드는 루트 노드)
		if (target->m_pParent != nullptr)
		{
			// 타겟 노드가 타겟 노드의 부모 노드의 왼쪽 자식인 경우
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