#pragma once

#include "Game/Point.h"

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