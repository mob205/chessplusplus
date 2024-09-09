#include <iostream>
#include "Game/Point.h"

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

Point& Point::operator+=(const Point& other)
{
	this->rank += other.rank;
	this->file += other.file;
	return *this;
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

Point operator*(const Point& p1, int scalar)
{
	return { p1.rank * scalar, p1.file * scalar };
}

Point operator*(int scalar, const Point& p1)
{
	return p1 * scalar;
}