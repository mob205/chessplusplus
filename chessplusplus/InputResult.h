#pragma once

#include "Point.h"

struct InputResult
{
	enum Result
	{
		POINT,
		SAVE,
		QUIT,
		UNDO
	};

	Point point{};
	Result result{};
};