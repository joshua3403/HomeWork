#include "stdafx.h"
#include "CExceptClass.h"
#include "CMessage.h"

int main()
{
	CMessage test;

	BYTE byte1 = 1;
	char char1 = 10;
	short short1 = 153;
	double double1 = 12351236;
	double double2 = 123412351235;

	try
	{
		test << byte1 << char1 << short1 << double1 << double2;
	}
	catch (CExceptClass* exp)
	{
		exp->PrintInput(test.GetDataSize());
		delete exp;
	}

	try
	{
		test >> byte1 >> char1 >> short1 >> double1 >> double2;
	}
	catch (CExceptClass* exp)
	{
		exp->PrintInput(test.GetDataSize());
		delete exp;
	}

	printf("%d, %d, %d, %f, %f", byte1, char1, short1, double1, double2);



	return 0;
}