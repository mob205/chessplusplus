#pragma once

#include <string_view>

namespace PieceEnums
{
	constexpr std::string_view pieceNames[]{ "None", "Pawn", "Knight", "Bishop", "Rook", "Queen", "King" };
	constexpr char pieceSymbols[]{ '\0', 'P', 'N', 'B', 'R', 'Q', 'K' };

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