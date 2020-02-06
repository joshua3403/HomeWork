#include "stdafx.h"
#include "CList(STL).h"
#include <list>


using namespace std;

int main()
{

	CList<int> test;

	test.push_front(5);
	test.push_back(19);

	CList<int>::iterator it = test.begin();

	cout << *it << endl;

	test.clear();


	test.push_back(15);
	test.push_back(1325);
	
	it = test.end();

	test.erase(it);

	cout << *test.end() << endl;


	cout << "³¡³µ½À´Ï´Ù" << endl;
	
	return 0;
}