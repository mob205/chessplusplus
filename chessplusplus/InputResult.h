#pragma once

#include "Point.h"

struct InputResult
{
	enum Result
	{
		POINT,
		SAVE,
		QUIT
	};

	Point point{};
	Result result{};
};