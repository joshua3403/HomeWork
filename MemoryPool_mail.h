/*---------------------------------------------------------------

	procademy MemoryPool.

	�޸� Ǯ Ŭ���� (������Ʈ Ǯ / ��������Ʈ)
	Ư�� ����Ÿ(����ü,Ŭ����,����)�� ������ �Ҵ� �� ��������.

	- ����.

	procademy::CMemoryPool<DATA> MemPool(300, FALSE);
	DATA *pData = MemPool.Alloc();

	pData ���

	MemPool.Free(pData);

				
----------------------------------------------------------------*/
#pragma once
#include "stdafx.h"

template <class DATA>
class CFreeList
{
private:

	/* **************************************************************** */
	// �� �� �տ� ���� ��� ����ü.
	/* **************************************************************** */
	struct st_BLOCK_NODE
	{
		st_BLOCK_NODE()
		{
			stpNextBlock = NULL;
		}

		st_BLOCK_NODE *stpNextBlock;
	};




public:

	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Parameters:	(int) �ʱ� �� ����.
	//				(bool) Alloc �� ������ / Free �� �ı��� ȣ�� ����
	// Return:
	//////////////////////////////////////////////////////////////////////////
			CFreeList(int iBlockNum, bool bPlacementNew = false);
	virtual	~CFreeList();


	//////////////////////////////////////////////////////////////////////////
	// �� �ϳ��� �Ҵ�޴´�.  
	//
	// Parameters: ����.
	// Return: (DATA *) ����Ÿ �� ������.
	//////////////////////////////////////////////////////////////////////////
	DATA	*Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// ������̴� ���� �����Ѵ�.
	//
	// Parameters: (DATA *) �� ������.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(DATA *pData);


	//////////////////////////////////////////////////////////////////////////
	// ���� Ȯ�� �� �� ������ ��´�. (�޸�Ǯ ������ ��ü ����)
	//
	// Parameters: ����.
	// Return: (int) �޸� Ǯ ���� ��ü ����
	//////////////////////////////////////////////////////////////////////////
	int		GetAllocCount(void) { return m_iAllocCount; }

	//////////////////////////////////////////////////////////////////////////
	// ���� ������� �� ������ ��´�.
	//
	// Parameters: ����.
	// Return: (int) ������� �� ����.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return m_iUseCount; }


	// ���� ������� ��ȯ�� (�̻��) ������Ʈ ���� ����.

	st_BLOCK_NODE *_pFreeNode;

private :
	int m_iAllocCount;
	int m_iUseCount;
	bool  m_bUsingPlacementNew;
};



template<class DATA>
inline CFreeList<DATA>::CFreeList(int iBlockNum, bool bPlacementNew)
{
	m_iAllocCount = iBlockNum;
	m_iUseCount = 0;
	_pFreeNode = new st_BLOCK_NODE;
	st_BLOCK_NODE temp;
	int count = m_iAllocCount;
	if (m_iAllocCount != 0)
	{
		if (bPlacementNew)
		{
			while (count > 0)
			{
				void* newBlock = malloc(sizeof(st_BLOCK_NODE) + sizeof(DATA));
				newBlock = &temp;
				
				//(*(st_BLOCK_NODE*)newBlock)->
				*(char*)newBlock += sizeof(st_BLOCK_NODE);
				DATA* newObject = new(newBlock) DATA;
				_pFreeNode->stpNextBlock = newBlock;
				count--;
			}

		}
	}

}

template<class DATA>
inline CFreeList<DATA>::~CFreeList()
{
}

template<class DATA>
inline DATA* CFreeList<DATA>::Alloc(void)
{
	return NULL;
}

template<class DATA>
inline bool CFreeList<DATA>::Free(DATA* pData)
{
	return false;
}


