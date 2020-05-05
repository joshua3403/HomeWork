#include  "CFreeList.h"



template<class DATA>
inline CFreeList<DATA>::CFreeList(int iBlockNum, bool bPlacementNew)
{
}

template<class DATA>
inline CFreeList<DATA>::~CFreeList()
{
}

template<class DATA>
DATA* CFreeList<DATA>::Alloc(void)
{
	return nullptr;
}

template<class DATA>
bool CFreeList<DATA>::Free(DATA* pData)
{
	return false;
}
