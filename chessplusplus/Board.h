#pragma once

#include <vector>
#include <array>
#include <iostream>

#include "Settings.h"
#include "Point.h"

class Piece;

using Row = std::array<Piece*, Settings::g_boardSize>;

class Board
{
public:
	Row& operator[](int index);
	Piece* operator[](Point pos);
	friend std::ostream& operator<<(std::ostream& out, Board& board);

private:
	std::array<Row, Settings::g_boardSize> board{};
};