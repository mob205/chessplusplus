#include <iostream>
#include "Point.h"

std::istream& operator>> (std::istream& in, Point& point)
{
	char file{};
	int rank{};

	in >> file >> rank;

	if (in)
		point = Point{ rank - 1, file - 'a'};

	return in;
}

std::ostream& operator<< (std::ostream& out, const Point& point)
{
	out << static_cast<char>(point.file + 'a') << point.rank + 1;
	return out;
}

Point operator+(const Point& p1, const Point& p2)
{
	return { p1.rank + p2.rank, p1.file + p2.file };
}

Point operator-(const Point& p1, const Point& p2)
{
	return { p1.rank - p2.rank, p1.file - p2.file };
}

Point operator-(const Point& p1)
{
	return { -p1.rank, -p1.file };
}

bool operator==(const Point& p1, const Point& p2)
{
	return (p1.rank == p2.rank) && (p1.file == p2.file);
}

