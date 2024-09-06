#include <cassert>

#include "Board.h"
#include "Piece.h"

Row& Board::operator[](int idx)
{
	assert(0 <= idx && idx < Settings::g_boardSize && "Attempted to index board by an out-of-bounds index");
	return board[idx];
}
Piece* Board::operator[](Point point)
{
	assert(point.isInBounds() && "Attempted to index board by an out-of-bounds point.");
	return board[point.rank][point.file];
}

std::ostream& operator<<(std::ostream& out, Board& board)
{
	out << "   ";
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		out << " " << static_cast<char>('a' + i) << " ";
	}

	out << "\n   ";
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		out << "---";
	}
	out << "\n";
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		out << Settings::g_boardSize - i << " |";
		for (int j = 0; j < Settings::g_boardSize; ++j)
		{
			if (board[i][j])
			{
				out << *board[i][j];
			}
			else
			{
				out << "   ";
			}
		}
		out << "|\n";
	}
	out << "   ";
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		out << "---";
	}
	out << "\n";
	return out;
}