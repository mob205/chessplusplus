#include "ConsoleInput/ConsolePlayer.h"
#include "Game/Game.h"
#include "Board/Board.h"
#include "ConsoleInput/Input.h"
#include "Board/BoardHelpers.h"

namespace IO
{
	void playGame(Game& game)
	{
		const Board& board{ game.getBoard() };
		InputResult start{};
		while (start.result != InputResult::QUIT)
		{
			static std::unique_ptr<Move> savedMove{};

			PieceEnums::Team currentTeam{ game.getCurrentTeam() };
			PieceEnums::Team oppTeam{ getOppositeTeam(currentTeam) };

			std::cout << '\n' << board << '\n';

			std::cout << "Turn " << (game.getCurrentTurn() / 2) + 1 << '\n';
			std::cout << (currentTeam ? "Black" : "White") << "'s turn!\n";
			std::cout << "Select a piece to move, or type 'QUIT' to quit.\n";


			start = IO::getTileInput();
			if (start.result == InputResult::QUIT)
			{
				break;
			}

			else if (start.result == InputResult::SAVE)
			{
				if (game.getSerializer().saveGame(IO::getSaveName()))
				{
					std::cout << "Successfully saved the game.\n";
				}
				else
				{
					std::cout << "Failed to save the game.\n";
				}
				continue;
			}
			else if (start.result == InputResult::UNDO)
			{
				if (game.undoMove())
				{
					std::cout << "Undo successful.\n";
				}
				else
				{
					std::cout << "No move to undo!\n";
				}
				continue;
			}

			// Must be point result
			if (!ownsPiece(board, start.point, currentTeam))
			{
				std::cout << "No owned piece is on this square!\n";
				continue;
			}


			std::cout << "Selected a " << board[start.point]->getName() << "\n\n";


			std::cout << "Select a tile to move to, or type 'QUIT' to unselect the piece.\n";
			InputResult end = IO::getTileInput();

			if (end.result == InputResult::QUIT)
			{
				continue;
			}
			if (end.result != InputResult::POINT)
			{
				std::cout << "Invalid input.\n";
				continue;
			}

			MoveResult res{ game.processTurn(start.point, end.point) };

			if (!res)
			{
				switch (res.reasonFailed)
				{
				case MoveResult::MoveFailReason::InvalidPiece:
					std::cout << "Invalid move. There is no owned piece on this tile.\n";
					break;
				case MoveResult::MoveFailReason::Check:
					std::cout << "Invalid move. King in check must be defended.\n";
					break;
				case MoveResult::MoveFailReason::Pinned:
					std::cout << "Invalid move. This piece is pinned and must defend the king.\n";
					break;
				case MoveResult::MoveFailReason::NotInSet:
					std::cout << "Invalid move.";
					break;
				case MoveResult::MoveFailReason::NeedsInput:
					// NeedsInput has lowest priority - if this is the reason it fails, then it must mean the move is otherwise valid
					res = game.processTurn(start.point, end.point, IO::getPromotionType());
					break;
				}

				continue;
			}

			// Valid move
			switch (res.type)
			{
			case MoveResult::Type::Standard:
				if (res.standard.capturedPiece)
				{
					std::cout << "Captured an enemy " << PieceEnums::pieceNames[res.standard.capturedPiece] << '\n';
				}
				break;
			case MoveResult::Type::Castle:
				std::cout << "Castled!";
				break;
			case MoveResult::Type::EnPassant:
				std::cout << "EN PASSANT!!!!";
				break;
			case MoveResult::Type::Promotion:
				if (res.promotion.capturedPiece)
				{
					std::cout << "Captured an enemy " << PieceEnums::pieceNames[res.promotion.capturedPiece] << '\n';
				}
				std::cout << "Promoted pawn to " << PieceEnums::pieceNames[res.promotion.promotionType] << '\n';
				break;
			}

			switch (res.oppStatus)
			{
			case MoveResult::OpponentStatus::Check:
				std::cout << "Check!\n";
				break;
			case MoveResult::OpponentStatus::Checkmate:
				std::cout << "CHECKMATE! " << (currentTeam ? "Black" : "White") << " WINS!!\n";
				std::cout << board << '\n';
				return;
			case MoveResult::OpponentStatus::Stalemate:
				std::cout << "Stalemate.\n";
				std::cout << board << '\n';
				return;
			}
		}
	}
}