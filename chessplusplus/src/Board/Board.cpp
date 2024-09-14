#include <cassert>

#include "Board/Board.h"
#include "Piece/Piece.h"


Row& Board::operator[](int idx)
{
	assert(0 <= idx && idx < Settings::g_boardSize && "Attempted to index board by an out-of-bounds index");
	return board[idx];
}
std::unique_ptr<Piece>& Board::operator[](Point point)
{
	assert(point.isInBounds() && "Attempted to index board by an out-of-bounds point.");
	return board[point.rank][point.file];
}
const std::unique_ptr<Piece>& Board::operator[](Point point) const
{
	assert(point.isInBounds() && "Attempted to index board by an out-of-bounds point.");
	return board[point.rank][point.file];
}

std::ostream& operator<<(std::ostream& out, const Board& board)
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
	for (int rank = Settings::g_boardSize - 1; rank >= 0; --rank)
	{
		out << rank + 1 << " |";
		for (int file = 0; file < Settings::g_boardSize; ++file)
		{
			if (board[{rank, file}])
			{
				out << *board[{rank, file}];
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

