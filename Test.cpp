#include "stdafx.h"
#include "My_New.h"

int main()
{
	int* temp = new int;
	int* t = nullptr;

	delete temp;
	delete t;

	return 0;
}