#pragma once

#include <iostream>
#include <cstdio>

#define MAX_X_POSITION 75

class BaseObject
{
protected:
	int _X;
	
public:
	BaseObject(int x) : _X(x){ }
	virtual bool Action() = 0;
	virtual void Draw() = 0;
};
