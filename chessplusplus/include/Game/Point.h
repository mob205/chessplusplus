#pragma once
#include <iostream>
#include <vector>
#include <format>

#include "Game/Settings.h"

struct Point
{
	int rank{};
	int file{};

	// Returns if the tile described is within the board bounds
	bool isInBounds() const
	{
		return(0 <= rank && rank < Settings::boardSize && 0 <= file && file < Settings::boardSize);
	}

	Point& operator+=(const Point& other);

	friend Point operator*(const Point& point, int scalar);
	
};

// Hashing functor for using a Point as key in unordered_map
struct PointHash
{
	std::size_t operator()(const Point& point) const
	{
		int sum = point.rank + point.file;
		return static_cast<size_t>(((sum) * (sum + 1)) / (2 + point.file));
	}
};

template<>
struct std::formatter<Point>
{
	constexpr auto parse(std::format_parse_context& ctx)
	{
		return ctx.begin();
	}
	auto format(const Point& value, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "{}{}", static_cast<char>(value.file + 'a'), value.rank + 1);
	}
};

std::istream& operator>> (std::istream& in, Point& point);

std::ostream& operator<< (std::ostream& out, const Point& point);

Point operator+(const Point& p1, const Point& p2);

Point operator-(const Point& p1, const Point& p2);

Point operator-(const Point& p1);

bool operator==(const Point& p1, const Point& p2);

