#pragma once

#include "Game/Point.h"
#include "Piece/PieceEnums.h"
#include "Move/MoveInput.h"

class Game;


namespace Engine
{
	MoveInput generateMove(const Game& game, PieceEnums::Team team);
	
}

