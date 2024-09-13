#pragma once

#include <string_view>

namespace PieceEnums
{
	constexpr std::string_view pieceNames[]{ "None", "Pawn", "Knight", "Bishop", "Rook", "Queen", "King" };

	enum Team
	{
		White,
		Black,
		MaxTeams
	};
	enum Type
	{
		None,
		Pawn,
		Knight,
		Bishop,
		Rook,
		Queen,
		King,
		MaxTypes
	};
}