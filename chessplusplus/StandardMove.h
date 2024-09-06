#pragma once
#include "IMove.h"
#include "Point.h"

class StandardMove : public IMove
{
public:
	StandardMove(Point start, Point end);

	void ExecuteMove(Board& board) const override;
	~StandardMove() override = default;

private:
	Point start{};
	Point end{};
};