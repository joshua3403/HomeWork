/*---------------------------------------------------------------

	procademy MemoryPool.

	메모리 풀 클래스 (오브젝트 풀 / 프리리스트)
	특정 데이타(구조체,클래스,변수)를 일정량 할당 후 나눠쓴다.

	- 사용법.

	procademy::CMemoryPool<DATA> MemPool(300, FALSE);
	DATA *pData = MemPool.Alloc();

	pData 사용

	MemPool.Free(pData);

				
----------------------------------------------------------------*/
#pragma once
#include "stdafx.h"

template <class DATA>
class CFreeList
{
private:

	/* **************************************************************** */
	// 각 블럭 앞에 사용될 노드 구조체.
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
	// 생성자, 파괴자.
	//
	// Parameters:	(int) 초기 블럭 개수.
	//				(bool) Alloc 시 생성자 / Free 시 파괴자 호출 여부
	// Return:
	//////////////////////////////////////////////////////////////////////////
			CFreeList(int iBlockNum, bool bPlacementNew = false);
	virtual	~CFreeList();


	//////////////////////////////////////////////////////////////////////////
	// 블럭 하나를 할당받는다.  
	//
	// Parameters: 없음.
	// Return: (DATA *) 데이타 블럭 포인터.
	//////////////////////////////////////////////////////////////////////////
	DATA	*Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// 사용중이던 블럭을 해제한다.
	//
	// Parameters: (DATA *) 블럭 포인터.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(DATA *pData);


	//////////////////////////////////////////////////////////////////////////
	// 현재 확보 된 블럭 개수를 얻는다. (메모리풀 내부의 전체 개수)
	//
	// Parameters: 없음.
	// Return: (int) 메모리 풀 내부 전체 개수
	//////////////////////////////////////////////////////////////////////////
	int		GetAllocCount(void) { return m_iAllocCount; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return m_iUseCount; }


	// 스택 방식으로 반환된 (미사용) 오브젝트 블럭을 관리.

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


