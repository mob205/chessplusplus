#include "GUI/TurnLog.h"
#include "GUI/TextObject.h"
#include "Move/MoveResult.h"
#include <format>
#include <sstream>

constexpr int charsPerLine{ 25 };

namespace GUI
{
	TurnLog::TurnLog(int logSize, const sf::Font& font, const std::string& name)
		: Object{ name }, size{ logSize }, 
		textObj{ std::make_shared<TextObject>("Log Text", "", font, 25) }
	{
		addChild(textObj);
	}

	void TurnLog::logMessage(const std::string& message)
	{
		logMessages.push_front(message);
		if (logMessages.size() > size)
		{
			logMessages.pop_back();
		}
		updateLogText();
	}

	void TurnLog::logMove(const MoveResult& move)
	{

		std::string_view pieceName = PieceEnums::pieceNames[move.movedPieceType];
		logMessage(std::format("{} to {}{}.", pieceName, std::string(1, move.end.file + 'a'), move.end.rank + 1));

		if (move.capturedPieceType != PieceEnums::None)
		{
			std::string_view captureTypeName = PieceEnums::pieceNames[move.capturedPieceType];
			logMessage(std::format("Captured enemy {}.", captureTypeName));
		}

		switch (move.moveType)
		{
		case MoveResult::Type::Castle:
			logMessage("Castled!");
			break;

		case MoveResult::Type::Promotion:
		{
			std::string_view promotedTypeName = PieceEnums::pieceNames[move.promotion.promotionType];
			logMessage(std::format("Pawn promoted to {}.", promotedTypeName));
		}
			break;

		case MoveResult::Type::EnPassant:
			logMessage(std::format("EN PASSANT!!!"));
			break;
		}
	}

	void TurnLog::updateLogText()
	{
		std::stringstream logText{};

		int numEmptyLines = size - static_cast<int>(logMessages.size());

		for (int i = 0; i < numEmptyLines; ++i)
		{
			logText << "\n";
		}

		for (int i = static_cast<int>(logMessages.size()) - 1; i >= 0; --i)
		{
			logText << logMessages[i] << '\n';
		}

		textObj->setText(logText.str());
	}

	void TurnLog::clearMessages()
	{
		logMessages.clear();
		updateLogText();
	}
}

