#pragma once
#include <iostream>
#include <vector>

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

struct PointHash
{
	std::size_t operator()(const Point& point) const
	{
		int sum = point.rank + point.file;
		return static_cast<size_t>(((sum) * (sum + 1)) / (2 + point.file));
	}
};

std::istream& operator>> (std::istream& in, Point& point);

std::ostream& operator<< (std::ostream& out, const Point& point);

Point operator+(const Point& p1, const Point& p2);

bool operator==(const Point& p1, const Point& p2);

