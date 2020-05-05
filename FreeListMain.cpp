#include "stdafx.h"
#include "MemoryPool_mail.h"
#include "BaseObject.h"

int main()
{
	CFreeList<BaseObject> MemoryPool(1, true);
	return 0;
}