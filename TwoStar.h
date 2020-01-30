#pragma once

#include <iostream>
#include <cstdio>
#include "BaseObject.h"

class TwoStar : public BaseObject
{
public:

	TwoStar(int x) : BaseObject(x)
	{
	}
	~TwoStar()
	{
	}

	virtual bool Action()
	{
		if (this->_X >= MAX_X_POSITION - 2)
			return true;
		else
		{
			_X += 2;
			return false;
		}
	}

	virtual void Draw() override
	{
		for (int i = 0; i < this->_X; ++i)
		{
			printf(" ");
		}
		printf("**");
	}
};