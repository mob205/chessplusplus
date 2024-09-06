#pragma once
#include "Settings.h"

struct Point
{
	int rank{};
	int file{};

	bool isInBounds() const
	{
		return(0 <= rank && rank < Settings::g_boardSize && 0 <= file && file < Settings::g_boardSize);
	}
};

std::istream& operator>> (std::istream& in, Point& point);

std::ostream& operator<< (std::ostream& out, Point& point);

