#pragma once
#include "Point.h"

class IMove
{
public:
	virtual void ExecuteMove() = 0;
	virtual ~IMove() = 0;
};