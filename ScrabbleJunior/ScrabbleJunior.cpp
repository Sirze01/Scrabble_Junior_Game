#include "Game.h"

struct PlayerData {
	std::string name;
	int color;
	bool isBot;
};

/**
 * Ask what player must go first.
 * @param board - board on the screen at the moment. Uses dimension to write card view.
 * @param nPlayers - number of players of the game to be created.
 * @param playerNames - names of the players.
 * @param playerColors - colors of the players.
 * @return vector position of the player who'll start.
 */
size_t askPlayFirst(const Board &board, size_t nPlayers,
	const std::vector<std::string> &playerNames, const std::vector<int> &playerColors) {
	{ // card view for available players and random tip
		std::stringstream toWrite;
		std::vector<std::string> sentences = {
			"Choose one of the players",
			"or type 'random' to randomize.\n",
		};

		for (const auto& i : sentences) {
			toWrite << TOPIC << i << "\n";
		}

		for (size_t i = 0; i < nPlayers; ++i) {
			Util::outputForeColor(toWrite, playerColors.at(i), TOPIC);
			toWrite << ARROW << SPACE << playerNames.at(i) << "\n";
		}

		Util::writeCardView(board.getDimensions().line, board.getDimensions().col, toWrite);
	}

	{ //ask for user preference
		std::string playerName;
		for (;;) {
			Util::paddingAndTopic(WHITE, true); std::cout << "Who should go first? ";
			std::getline(std::cin, playerName);
			
			Util::cleanBuffer();
			Util::stripSpecialChars(playerName);
			Util::stripUnnecessarySpaces(playerName);

			if (playerName == "random") {
				int randomPos = Util::randomBetween(0, static_cast<int>(nPlayers - 1));
				Util::paddingAndTopic(playerColors.at(randomPos), true); std::cout << playerNames.at(randomPos) << " won the draw.\n";
				Util::paddingAndTopic(playerColors.at(randomPos), false); std::cout << "Press enter to start!\n";
				Util::askEnter();
				return randomPos;
			}

			Util::upperNameInitials(playerName);
			for (size_t i = 0; i < nPlayers; ++i) {
				if (playerName == playerNames.at(i)) return i;
			}

			Util::paddingAndTopic(RED, true); std::cout << "We could not find a player with that exact name. Please try again.\n";
		}
	}
}

/**
 * Checks the validity of a filename.
 * @param filename - name of the file to check
 * @return true - file exists
 * @return false - file does not exist in the current directory or can't be open
 */
bool exists(const std::string &filename) {
	std::ifstream file(filename);
	return file.is_open();
}

/**
 * Ask name of the board to be used in game
 * @param board - board on the screen at the moment. Uses dimension to write card view.
 * @return 
 */
std::string askBoardFileName(const Board &board) {
	{ //ask view for bb promotion
		std::stringstream toWrite;
		std::vector<std::string> sentences = {
			"Tip\n",
			"You can use our companion",
			"Board Builder programme",
			"to create your own board!"
		};

		for (const std::string &str : sentences) {
			toWrite << TOPIC << str << "\n";
		}

		Util::writeCardView(board.getDimensions().line, board.getDimensions().col, toWrite);
	}

	{ //ask file name
		std::string fileName;
		for (;;) {
			Util::paddingAndTopic(WHITE, true); std::cout << "Import board from file: ";
			std::getline(std::cin, fileName);
			Util::cleanBuffer();

			if (!exists(fileName)) {
				Util::paddingAndTopic(RED, true); std::cout << "We could not find that file. Please try again.\n";
			}
			else {
				Board testBoard(fileName);

				if (testBoard.getNonEmptyChars().size() < MIN_BOARD_LETTERS_WARNING) {
					std::string userAns;
					for (;;) { //exit when user goes back or agrees with the condition
						Util::paddingAndTopic(RED, true);
						std::cout << "That board hasn't got enough letters to create a fair 4 player game. Proceed? ";
						std::getline(std::cin, userAns); Util::cleanBuffer();
						Util::stripUnnecessarySpaces(userAns); Util::lowerCase(userAns);

						if (userAns == "yes" || userAns == "y") return fileName;
						if (userAns == "no" || userAns == "n") break; //ask file again
						else {
							Util::paddingAndTopic(BLUE, true);
							std::cout << "Please answer 'yes' or 'no'.\n";
						}
					}
				} else return fileName; //board ok
			}
		}
	}
}

/**
 * Ask the user to create a player with all its attributes
 * @param position - position of the player to be appended to the array
 * @param board - board on the screen at the moment. Uses dimension to write card view
 * @param forbiddenNames - names which already have been chosen and can't now
 * @param forbiddenColors - names which already have been chosen and can't now
 * @return struct with all data necessary to then instanciate a object of type Player
 */
PlayerData askPlayer(int position, const Board& board,
	const std::vector<std::string> &forbiddenNames, const std::vector<int> &forbiddenColors) {

	PlayerData player = { "Unknown", WHITE, false };

	{ //card view for bot tip
		std::stringstream toWrite;
		std::vector<std::string> sentences = {
			"If you want this player to be a bot",
			"include 'Computer' or 'Bot' in his name."
		};

		for (const auto& sentence : sentences) toWrite << TOPIC << sentence << "\n";
		Util::writeCardView(board.getDimensions().line, board.getDimensions().col, toWrite);
	}

	{ //ask name
		std::string name;
		bool isBot = false;
		position++;

		for (;;) {
			Util::paddingAndTopic(WHITE, true); std::cout << "Player " << position << " name: ";
			std::getline(std::cin, name);
			Util::cleanBuffer();
			Util::stripUnnecessarySpaces(name);

			if (name == "random") {
				Util::paddingAndTopic(RED, true); std::cout << "That'll be a reserved keyword. Please choose a different name.\n";
				continue;
			}

			Util::upperNameInitials(name);

			if (name.empty() || name.size() > MAX_PLAYER_NAME_SIZE) {
				Util::paddingAndTopic(RED, true); std::cout << "Please do not input large or empty names.\n";
			}
			else if (!Util::isAlpha(name, true)) {
				Util::paddingAndTopic(RED, true); std::cout << "Please do not use digits or special characters.\n";
			}
			else if (std::find(forbiddenNames.begin(), forbiddenNames.end(), name) != forbiddenNames.end()) {
				Util::paddingAndTopic(RED, true); std::cout << "Another player has already chosen that name. Try again.\n";
			}
			else if (name.find("Computer") != std::string::npos || name.find("Bot") != std::string::npos) {
				isBot = true;
				Util::paddingAndTopic(BLUE, true); std::cout << "This player will be a bot.\n";
				break;
			}
			else break;
		}

		player.name = name;
		player.isBot = isBot;
	}


	{ //card view for colors available
		std::stringstream toWrite;
		std::vector<int> colors = { RED,GREEN,BLUE,PINK,ORANGE };
		std::vector<std::string> colorNames = { "RED","GREEN","BLUE","PINK","ORANGE" };

		toWrite << TOPIC << colors.size() - forbiddenColors.size() << " colors available\n\n";

		for (size_t i = 0; i < colors.size(); ++i) {
			if (std::find(forbiddenColors.begin(), forbiddenColors.end(), colors.at(i)) != forbiddenColors.end()) continue;
			Util::outputForeColor(toWrite, colors.at(i), TOPIC);
			toWrite << ARROW << SPACE << colorNames.at(i) << "\n";
		}
		Util::writeCardView(board.getDimensions().line, board.getDimensions().col, toWrite);
	}

	{ //ask color
		std::string colorName;
		int color;

		for (;;) { //ask color
			Util::paddingAndTopic(WHITE, true); std::cout << "Player " << position << " color: ";
			std::getline(std::cin, colorName);
			
			Util::cleanBuffer();
			Util::stripSpecialChars(colorName);
			Util::stripUnnecessarySpaces(colorName);
			Util::lowerCase(colorName);

			if (colorName == "red" || colorName == "r") color = RED;
			else if (colorName == "green" || colorName == "g") color = GREEN;
			else if (colorName == "blue" || colorName == "b") color = BLUE;
			else if (colorName == "pink" || colorName == "p") color = PINK;
			else if (colorName == "orange" || colorName == "o") color = ORANGE;
			else {
				Util::paddingAndTopic(RED, true); std::cout << "We did not recognize that color. Please try again.\n";
				continue;
			}

			if (std::find(forbiddenColors.begin(), forbiddenColors.end(), color) != forbiddenColors.end()) {
				Util::paddingAndTopic(RED, true); std::cout << "Another player has already chosen that color. Try again.\n";
			}
			else break;
		}
		player.color = color;
	}

	return player;
}

/**
 * Asks the user the number of the players to participate in the game.
 * @return size of the future vector of players
 */
int askNumberOfPlayers() {
	std::string input, errorMessage;
	int intInput;

	for (;;) {
		Util::paddingAndTopic(WHITE, true); std::cout << "Number of players: ";
		std::getline(std::cin, input);
		Util::cleanBuffer();
		Util::stripSpecialChars(input,true);
		Util::stripUnnecessarySpaces(input);

		if (!Util::isDigit(input)) {
			errorMessage = "Please input a valid number.";
		}
		else {
			intInput = std::stoi(input);
			if (intInput < 2 || intInput > 4) {
				errorMessage = "This game can only be played by 2 to 4 players.";
			}
			else break;
		}

		Util::paddingAndTopic(RED, true); std::cout << errorMessage << "\n";
	}

	return intInput;
}

/**
 * Shows the intro board and a intro message. Waits for enter.
 * @param introBoard - board to be shown.
 */
void printIntro(const Board& introBoard) {
	introBoard.show();

	std::vector<std::string> sentences = {
	"Welcome to our Scrabble Junior Game!",
	"We hope you'll have a great time with us.",
	"Press enter to create a new game!",
	};

	for (const auto& sentence : sentences) {
		Util::paddingAndTopic(BLUE, true); std::cout << sentence;
	}

	std::cout << "\n"; Util::askEnter();
}

/**
 * Calls auxiliary functions to create a new game. Then loops until game is not finished.
 */
int main()
{
	Util::setupConsole();
	Util::initRandom();

	auto clearAndShowBoard = [](const Board& board) {
		Util::clearConsole();
		board.show();
	};

	Board introBoard("intro_board.txt"); //if file does not exist, the constructor creates a default empty board

	std::vector<std::string> playerNames;
	std::vector<int> playerColors;
	std::vector<bool> botFlags;

	printIntro(introBoard);
	clearAndShowBoard(introBoard);

	std::string filename = askBoardFileName(introBoard);
	Board gameBoard(filename);
	clearAndShowBoard(gameBoard);

	int nPlayers = askNumberOfPlayers();
	for (int i = 0; i < nPlayers; ++i) {
		clearAndShowBoard(gameBoard);
		PlayerData player = askPlayer(i, gameBoard, playerNames, playerColors);
		playerNames.push_back(player.name);
		playerColors.push_back(player.color);
		botFlags.push_back(player.isBot);
	}
	clearAndShowBoard(gameBoard);

	int first = askPlayFirst(gameBoard, nPlayers, playerNames, playerColors);
	Game my_game(gameBoard, playerNames, playerColors, botFlags, first);

	for (;;) { //while game is going on
		my_game.moveHandler(1);
		if (my_game.hasFinished()) break;
		my_game.moveHandler(2);
		if (my_game.hasFinished()) break;
		my_game.nextTurn();
	}

	my_game.showEndMessage();
}