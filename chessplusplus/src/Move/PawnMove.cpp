#include "Move/PawnMove.h"
#include "Input/Input.h"
#include "Piece/Piece.h"
#include "Board/Board.h"

#include "Piece/Knight.h"
#include "Piece/Queen.h"
#include "Piece/Rook.h"
#include "Piece/Bishop.h"

void PawnMove::executeMove(Board& board)
{
	Move::executeMove(board);
	int promoRank{ board[end]->getTeam() ? 0 : Settings::g_boardSize - 1 };

	if (canPromote && board[end]->getPosition().rank == promoRank)
	{
		promotePawn(board);
	}
}

void PawnMove::promotePawn(Board& board)
{
	promotedPawn = std::move(board[end]);
	std::cout << "\nSelect a piece to promote to.\nQ - Queen | R - Rook | B - Bishop | N - Knight\n";

	Piece::Type promoType{ Input::getPromotionType() };
	Piece::Team team{ promotedPawn->getTeam() };
	switch (promoType)
	{
	case Piece::Knight:
		board[end] = std::make_unique<Knight>(end, team);
		break;
	case Piece::Queen:
		board[end] = std::make_unique<Queen>(end, team);
		break;
	case Piece::Rook:
		board[end] = std::make_unique<Rook>(end, team);
		break;
	case Piece::Bishop:
		board[end] = std::make_unique<Bishop>(end, team);
		break;
	}
	promotedPiece = board[end].get();
}

void PawnMove::undoMove(Board& board)
{
	promotedPiece = nullptr;
	if (promotedPawn)
	{
		board[end] = std::move(promotedPawn);
	}
	Move::undoMove(board);
}

void PawnMove::printMove() const
{
	if (captured)
	{
		std::cout << "Captured a " << captured->getName() << '\n';
	}
	if (promotedPiece)
	{
		std::cout << "Pawn promoted to " << promotedPiece->getName() << '\n';
	}
}
