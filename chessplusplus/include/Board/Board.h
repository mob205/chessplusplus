#pragma once

#include <vector>
#include <array>
#include <iostream>

#include "Game/Settings.h"
#include "Game/Point.h"
#include "Piece/Piece.h"
#include "Piece/Pawn.h"


using Row = std::array<std::unique_ptr<Piece>, Settings::boardSize>;

class Board
{
public:

	template<typename T, typename ...Args> requires std::is_base_of_v<Piece, T>
	void addPiece(Point pos, Args&&... args)
	{
		// Relies on fact that position is always first constructor argument of a piece
		board[pos.rank][pos.file] = std::make_unique<T>(std::forward<Args>(args)...);
		board[pos.rank][pos.file]->setPosition(pos);
	}

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