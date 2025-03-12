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
		: game{ game }
	{}

	bool saveGame(const std::string& savename) const;
	LoadGameResult loadGame(const std::string& savename) const;
	char readPromoType(std::istream& fp) const;

	// Copies from source to this serializer's associated game
	void copyGame(const Game& source) const;

private:
	Game& game;

	bool saveGame(std::ostream& out) const;
	GameSerializer::LoadGameResult loadGame(std::istream& in) const;

	static constexpr char saveDirectory[] = "Saves/";
	static constexpr char extension[] = ".save";
};