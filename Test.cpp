#include "stdafx.h"
#include "Profiler.h"

class tempClass
{
public:
	tempClass()
	{
		std::cout << "������ ����" << std::endl;
	}

	~tempClass()
	{
		std::cout << "�Ҹ��� ����" << std::endl;
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

	std::cout << "�Ϸ�" << std::endl;
	return 0;
}