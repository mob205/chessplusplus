#pragma once

#include <vector>
#include <array>
#include <iostream>

#include "Game/Settings.h"
#include "Game/Point.h"
#include "Piece/Piece.h"


using Row = std::array<std::unique_ptr<Piece>, Settings::boardSize>;

class Board
{
public:
	// Access row from board by rank index
	Row& operator[](int index);

	// Access a piece from the board by rank and file
	std::unique_ptr<Piece>& operator[](Point pos);
	const std::unique_ptr<Piece>& operator[](Point pos) const;

	// Prints the board
	friend std::ostream& operator<<(std::ostream& out, const Board& board);

private:
	std::array<Row, Settings::boardSize> board{};
};