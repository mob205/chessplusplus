#pragma once

class Game;

class GameSerializer
{
public:
	GameSerializer(Game& game)
		:game{ game }
	{}

	bool saveGame(const std::string& savename) const;
	bool loadGame(const std::string& savename) const;

private:
	Game& game;

	static constexpr char saveDirectory[] = "Saves/";
	static constexpr char extension[] = ".save";
};