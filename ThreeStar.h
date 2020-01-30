#pragma once

#include <iostream>
#include <cstdio>
#include "BaseObject.h"

class ThreeStar : public BaseObject
{
public:

	ThreeStar(int x) : BaseObject(x)
	{
	}
	~ThreeStar()
	{
	}

	virtual bool Action()
	{
		if (this->_X >= MAX_X_POSITION - 3)
			return true;
		else
		{
			_X += 3;
			return false;
		}
	}

	virtual void Draw() override
	{
		for (int i = 0; i < this->_X; ++i)
		{
			printf(" ");
		}
		printf("***");
	}
};