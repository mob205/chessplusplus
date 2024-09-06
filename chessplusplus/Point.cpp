#include <iostream>
#include "Point.h"

std::istream& operator>> (std::istream& in, Point& point)
{
	char rank{};
	int file{};

	in >> rank >> file;

	if (in)
		point = Point{ tolower(rank) - 'a', file - 1 };

	return in;
}

std::ostream& operator<< (std::ostream& out, Point& point)
{
	out << static_cast<char>(point.rank + 'a') << point.file + 1;
	return out;
}