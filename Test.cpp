#include "stdafx.h"
#include "My_New.h"

class test
{
public:
	test()
	{
		std::cout << "持失切" << std::endl;
	}

	~test()
	{
		std::cout << "社瑚切" << std::endl;
	}
};

int main()
{


	test* temp_array = new test[10];
	char* temp = new char;

	delete temp_array;
	return 0;
}