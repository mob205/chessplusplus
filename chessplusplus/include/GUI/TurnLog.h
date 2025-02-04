#pragma once

#include "GUI/Object.h"
#include "GUI/TextObject.h"
#include <deque>

class sf::Font;
struct MoveResult;

namespace GUI
{
	class TurnLog : public Object
	{
	public:
		TurnLog(int logSize, const sf::Font& font, const std::string& name = "Turn Log");

		void logMessage(const std::string& message);
		void logMove(const MoveResult& move);
		void clearMessages();
	private:
		int size;
		std::deque<std::string> logMessages;
		std::shared_ptr<TextObject> textObj;

		void updateLogText();
	};
}