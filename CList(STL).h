#pragma once

#include "stdafx.h"
template <typename T>
class CList
{

	class Node
	{
	public:
		T _data;
		Node* _Prev;
		Node* _Next;

		Node(T data = 0)
		{
			_data = data;
			_Prev = nullptr;
			_Next = nullptr;
		}
	};

public:

	class iterator
	{
	private:
		Node* _node;
		friend class CList;
	public:
		iterator(Node* node = nullptr)
		{
			this->_node = node;
		}

		// 후위 증감 연산
		iterator operator ++(int)
		{
			iterator temp(this);
			
			this->_node = this->_node->_Next;
			return temp;
		}

		// 전위 증감 연산
		iterator operator ++()
		{
			this->_node = this->_node->_Next;
			return *this;
		}

		// 후위 감소 연산
		iterator operator--(int)
		{
			iterator temp(this);
			this->_node = this->_node->_Prev;
			return temp;
		}

		// 전위 감소 연산
		iterator operator--()
		{
			this->_node = this->_node->_Prev;
			return *this;
		}

		// 포인터 연산
		const T& operator *()
		{
			return this->_node->_data;
		}

		// 복사 생성자
		iterator(const iterator& it)
		{
			_node = it._node;
		}

		// 대입 연산자
		void operator =(iterator it)
		{
			_node = it._node;
		}

		const bool operator == (iterator it)
		{
			return _node == it._node;
		}

		const bool operator != (iterator it)
		{
			return _node != it._node;
		}
	};

public:
	CList() : _size(0)
	{
		_head = new Node();
		_tail = new Node();
		_head->_Next = _tail;
		_tail->_Prev = _head;
	}

	~CList()
	{
		clear();
	}

	iterator begin()
	{
		return iterator(_head);
	}
	iterator end()
	{
		return iterator(_tail);
	}

	void push_back(T data)
	{
		Node* newNode = new Node(data);
		
		if (_size == 0)
		{
			newNode->_Next = _tail;
			_tail->_Prev = newNode;
			_head = newNode;
		}
		else
		{
			newNode->_Prev = _tail;
			_tail->_Next = newNode;
			_tail = newNode;
		}
		++_size;
	}

	void clear()
	{
		iterator itor(begin());
		Node* current = begin()._node;
		while (_size != 0)
		{
			Node* temp = current->_Next;
			delete current;
			current = temp;
			_size--;
		}
		
	}

	int size()
	{
		return _size;
	};

	bool empty()
	{
		if (_size > 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	};

	iterator erase(iterator itor)
	{
		if (_size > 0)
		{
			Node* node = itor._node;
			if (node->_Prev)		// 삭제하고자 하는 노드의 전 노드가 존재한다면
				node->_Prev->_Next = node->_Next;
			if (node->_Next)		// 삭제하고자 하는 노드의 다음 노드가 존재한다면
				node->_Next->_Prev = node->_Prev;

			Node* pCurrent = node->_Next;
			delete node;
			itor = pCurrent;
			--_size;
		}

		return itor;
	}


private:
	int _size = 0;
	Node* _head;
	Node* _tail;
};









