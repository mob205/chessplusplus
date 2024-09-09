#include "Move/PawnMove.h"
#include "Input/Input.h"
#include "Piece/Piece.h"
#include "Board/Board.h"

#include "Piece/Knight.h"
#include "Piece/Queen.h"
#include "Piece/Rook.h"
#include "Piece/Bishop.h"

void PawnMove::executeMove(Board& board, std::function<char()> inputCallback)
{
	Move::executeMove(board);
	int promoRank{ board[end]->getTeam() ? 0 : Settings::g_boardSize - 1 };

	if (inputCallback && board[end]->getPosition().rank == promoRank)
	{
		promotePawn(board, inputCallback);
	}
}

void PawnMove::promotePawn(Board& board, std::function<char()> getInput)
{
	promotedPawn = std::move(board[end]);

	extraInput = getInput();
	Piece::Team team{ promotedPawn->getTeam() };
	switch (extraInput)
	{
	case 'N':
		board[end] = std::make_unique<Knight>(end, team);
		break;
	case 'R':
		board[end] = std::make_unique<Rook>(end, team);
		break;
	case 'B':
		board[end] = std::make_unique<Bishop>(end, team);
		break;
	default:
		board[end] = std::make_unique<Queen>(end, team);
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
