#pragma once

#include <vector>
#include <array>
#include <iostream>

#include "Settings.h"
#include "Point.h"

class Piece;

using Row = std::array<std::unique_ptr<Piece>, Settings::g_boardSize>;
using PieceRef = std::unique_ptr<Piece>&;

class Board
{
public:
	// Access row from board by rank index
	Row& operator[](int index);

	// Access a piece from the board by rank and file
	std::unique_ptr<Piece>& operator[](Point pos);
	const std::unique_ptr<Piece>& operator[](Point pos) const;

	// Prints the board
	friend std::ostream& operator<<(std::ostream& out, Board& board);

private:
	std::array<Row, Settings::g_boardSize> board{};
};