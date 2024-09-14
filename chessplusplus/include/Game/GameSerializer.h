#pragma once

class Game;

class GameSerializer
{
public:
	enum LoadGameResult
	{
		LoadSuccessful,
		SaveNotFound,
		SaveInvalid
	};

	GameSerializer(Game& game)
		:game{ game }
	{}

	bool saveGame(const std::string& savename) const;
	LoadGameResult loadGame(const std::string& savename) const;
	char readPromoType(std::ifstream& fp) const;

private:
	Game& game;

	static constexpr char saveDirectory[] = "Saves/";
	static constexpr char extension[] = ".save";
};