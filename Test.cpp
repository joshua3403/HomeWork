#include "stdafx.h"
#include "Profiler.h"

class tempClass
{
public:
	tempClass()
	{
		std::cout << "持失切 持失" << std::endl;
	}

	~tempClass()
	{
		std::cout << "社瑚切 持失" << std::endl;
	}
};


using namespace std;

int main()
{

	Joshua::ProfilerList.PushBack(L"here");

	for (int i = 0; i < 10; ++i)
	{
		tempClass* t = new tempClass[3];


		delete[] t;
		Joshua::ProfilerList.PutNewData(L"here");

	}

	std::cout << "刃戟" << std::endl;
	return 0;
}