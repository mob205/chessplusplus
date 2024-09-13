#include "Move/PawnMove.h"
#include "Input/Input.h"
#include "Piece/Piece.h"
#include "Board/Board.h"
#include "Piece/PieceEnums.h"

#include "Piece/Knight.h"
#include "Piece/Queen.h"
#include "Piece/Rook.h"
#include "Piece/Bishop.h"

MoveResult PawnMove::executeMove(Board& board, std::function<char()> inputCallback)
{
	Move::executeMove(board);
	int promoRank{ board[end]->getTeam() ? 0 : Settings::g_boardSize - 1 };

	MoveResult res{};
	if (inputCallback && board[end]->getPosition().rank == promoRank)
	{
		PieceEnums::Type promoType{ promotePawn(board, inputCallback) };

		res.type = MoveResult::Type::Promotion;
		res.promotion = MoveResult::PromotionResult{ start, end, (captured) ? captured->getType() : PieceEnums::None, promoType };
		return res;
	}
	
	res.type = MoveResult::Type::Standard;
	res.standard = MoveResult::StandardResult{ start, end, (captured) ? captured->getType() : PieceEnums::None };
	return res;
}

PieceEnums::Type PawnMove::promotePawn(Board& board, std::function<char()> getInput)
{
	promotedPawn = std::move(board[end]);

	extraInput = getInput();
	PieceEnums::Team team{ promotedPawn->getTeam() };
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
	return promotedPiece->getType();
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
