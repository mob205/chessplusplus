#pragma once

#include "Game/Point.h"
#include "Piece/PieceEnums.h"
#include "Move/MoveInput.h"

class Game;


namespace Engine
{
	MoveInput generateMove(Game* game, PieceEnums::Team team);
	
}

