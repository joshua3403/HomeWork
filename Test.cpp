#include "stdafx.h"
#include "My_New.h"

class test
{
public:
	test()
	{
		std::cout << "������" << std::endl;
	}

	~test()
	{
		std::cout << "�Ҹ���" << std::endl;
	}
};

int main()
{


	test* temp_array = new test[10];
	char* temp = new char;

	delete temp_array;
	return 0;
}