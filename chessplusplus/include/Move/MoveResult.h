#pragma once
#include "Piece/PieceEnums.h"
#include "Game/Point.h"

struct MoveResult
{
	enum class MoveFailReason
	{
		None,			// Successful move
		InvalidPiece,   // Piece is not owned by player
		NotInSet,		// The requested move is not how the piece moves
		Pinned,			// The king is not in check and this move would put the king in check
		Check,			// The king is in check and this move does not take the king out of check
		NeedsInput,		// This move requires extra input. Other fail reasons take priority
	};
	enum class Type 
	{
		Standard,
		EnPassant,
		DoublePawn,
		Promotion,
		Castle
	};

	enum class OpponentStatus
	{
		Normal,
		Check,
		Checkmate,
		Stalemate,
	};

	MoveFailReason reasonFailed{};
	OpponentStatus oppStatus{};
	MoveResult::Type moveType{};
	PieceEnums::Type movedPieceType{ PieceEnums::None };
	PieceEnums::Type capturedPieceType{ PieceEnums::None };

	Point start{};
	Point end{};

	struct StandardResult
	{
	};

	struct EnPassantResult
	{
		// En passant is the only move to not capture on the ending tile
		Point capturePoint{};
	};
	
	struct DoublePawnResult
	{
	};

	struct PromotionResult
	{
		PieceEnums::Type promotionType{};
	};

	struct CastleResult
	{
		Point rookStart{};
		Point rookEnd{};
	};

	union
	{
		StandardResult		standard;
		EnPassantResult		enpassant;
		DoublePawnResult	doublePawn;
		PromotionResult		promotion;
		CastleResult		castle;
	};

	operator bool() const
	{
		return reasonFailed == MoveFailReason::None;
	}
};