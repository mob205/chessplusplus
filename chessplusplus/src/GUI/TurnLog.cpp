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
		textObj->setText("12345678902234567890323456789423456789\n");
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
		logMessage(std::format("{} to {}{}.", pieceName, move.start.file + 'a', move.start.rank + 1));
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

