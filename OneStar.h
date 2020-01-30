#pragma once

#include <iostream>
#include <cstdio>
#include "BaseObject.h"

class OneStar : public BaseObject
{
public:

	OneStar(int x) : BaseObject(x){}
	~OneStar(){}

	virtual bool Action()
	{
		if (this->_X >= MAX_X_POSITION - 1)
			return true;
		else
		{
			_X++;
			return false;
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < this->_X; ++i)
		{
			printf(" ");
		}
		printf("*");
	}
};