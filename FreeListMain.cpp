#include "stdafx.h"
#include "MemoryPool_mail.h"
#include "BaseObject.h"

class testClass
{
private:

	int t;

public:
	testClass()
	{
		t = 100;
		wprintf(L"testClass Constructor\n");
	}

	~testClass();

	void Print();
};

int main()
{
	CFreeList<testClass> MemoryPool(1, false);

	testClass* test1 = MemoryPool.Alloc();
	wprintf(L"test1 ");
	test1->Print();

	testClass* test2 = MemoryPool.Alloc();
	wprintf(L"test2 ");

	test1->Print();

	MemoryPool.Free(test2);
	return 0;
}



testClass::~testClass()
{
	wprintf(L"testClass Destroyer\n");

}

void testClass::Print()
{
	wprintf(L"%d\n", t);
}
