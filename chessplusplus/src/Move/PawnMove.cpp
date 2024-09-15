#include "Move/PawnMove.h"
#include "ConsoleInput/Input.h"
#include "Piece/Piece.h"
#include "Board/Board.h"
#include "Piece/PieceEnums.h"

#include "Piece/Knight.h"
#include "Piece/Queen.h"
#include "Piece/Rook.h"
#include "Piece/Bishop.h"

MoveResult PawnMove::executeMove(Board& board, char extraInput)
{
	Move::executeMove(board);
	int promoRank{ board[end]->getTeam() ? 0 : Settings::boardSize - 1 };

	MoveResult res{};
	if (board[end]->getPosition().rank == promoRank && extraInput)
	{
		PieceEnums::Type promoType{ promotePawn(board, extraInput) };

		res.type = MoveResult::Type::Promotion;
		res.promotion = MoveResult::PromotionResult{ start, end, (captured) ? captured->getType() : PieceEnums::None, promoType };
		return res;
	}
	else if (board[end]->getPosition().rank == promoRank && !extraInput)
	{
		res.reasonFailed = MoveResult::MoveFailReason::NeedsInput;
	}
	
	res.type = MoveResult::Type::Standard;
	res.standard = MoveResult::StandardResult{ start, end, (captured) ? captured->getType() : PieceEnums::None };
	return res;
}

PieceEnums::Type PawnMove::promotePawn(Board& board, char extraInput)
{
	promotedPawn = std::move(board[end]);

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
