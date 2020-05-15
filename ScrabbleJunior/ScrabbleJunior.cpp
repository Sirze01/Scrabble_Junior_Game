#include "../common/ConsoleSetup.h"
#include "Game.h"
#include "Pool.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>

//for shuffle purposes - should only be defined once
unsigned const SEED = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
std::mt19937 RANDOM_GENERATOR(SEED);

struct PlayerData {
	std::string name;
	int color;
	bool isBot;
};

int askPlayFirst(const Board &board, int nPlayers,
	const std::vector<std::string> &playerNames, const std::vector<int> &playerColors) {
	{
		std::stringstream toWrite;
		std::vector<std::string> sentences = {
			"Choose one of the players\n",
			"or type 'random' to randomize.\n\n",
		};

		for (const auto& i : sentences) {
			toWrite << TOPIC << i;
		}

		for (int i = 0; i < nPlayers; ++i) {
			outputForeColor(toWrite, playerColors.at(i), TOPIC);
			toWrite << ARROW << SPACE << playerNames.at(i) << "\n";
		}

		writeCardView(board.getDimensions().vLine, board.getDimensions().hColumn, toWrite);
	}

	std::string playerName;
	for (;;) {
		paddingAndTopic(WHITE, true); std::cout << "Who should go first? ";
		std::getline(std::cin, playerName); cleanBuffer();
		stripSpecialChars(playerName); stripSpaces(playerName);

		if (playerName == "random") {
			int randomPos = randomBetween(0, nPlayers - 1);
			paddingAndTopic(playerColors.at(randomPos), true); std::cout << playerNames.at(randomPos) << " won the draw.\n";
			paddingAndTopic(playerColors.at(randomPos), false); std::cout << "Press enter to start!\n";
			askEnter();
			return randomPos;
		}

		upperNameInitials(playerName);

		for (int i = 0; i < nPlayers; ++i) {
			if (playerName == playerNames.at(i)) return i;
		}

		paddingAndTopic(RED, true); std::cout << "We could not find a player with that exact name. Please try again.\n";
	}

	return 0;
}

bool exists(const std::string &filename) {
	std::ifstream file(filename);
	return file.is_open();
}

std::string askBoardFileName(const Board &board) {
	{
		std::stringstream toWrite;
		std::vector<std::string> sentences = {
			"Tip\n",
			"You can use our companion",
			"Board Builder program",
			"to create your own board!"
		};

		for (std::string str : sentences) {
			toWrite << TOPIC << str << "\n";
		}

		writeCardView(board.getDimensions().vLine, board.getDimensions().hColumn, toWrite);
	}

	std::string fileName;

	for (;;) {
		paddingAndTopic(WHITE, true); std::cout << "Import board from file: ";
		std::getline(std::cin, fileName); cleanBuffer();

		if (fileName == "") {
			paddingAndTopic(RED, true); std::cout << "Empty filenames are invalid. Please try again.\n";
		}
		else if (!exists(fileName)) {
			paddingAndTopic(RED, true); std::cout << "We could not find that file. Please try again.\n";
		}
		else {
			Board testBoard(fileName);

			if (testBoard.getNonEmptyChars().size() < MAX_BOARD_LETTERS_WARNING) {
				std::string userAns;

				for (;;) {
					paddingAndTopic(RED, true);
					std::cout << "That board hasn't got enough letters to create a fair 4 player game. Proceed? ";
					std::getline(std::cin, userAns); cleanBuffer();
					stripSpaces(userAns);
					for (auto& i : userAns) i = static_cast<char>(tolower(i));
					if (userAns == "yes" || userAns == "y") return fileName;
					if (userAns == "no" || userAns == "n") break;
					else {
						paddingAndTopic(BLUE, true); std::cout << "Please answer 'yes' or 'no'.\n";
						continue;
					}
				}
			}

			else break; //board ok
		}
	}

	return fileName;
}

PlayerData askPlayer(int position, const Board& board,
	const std::vector<std::string> &forbiddenNames, const std::vector<int> &forbiddenColors) {

	std::string name, colorName;
	bool isBot = false;
	int color = WHITE;
	position++;

	{
		std::stringstream toWrite;
		std::vector<std::string> sentences = {
			"If you want this player to be a bot\n",
			"include 'Computer' or 'Bot' in his name.\n"
		};

		for (const auto& sentence : sentences) toWrite << TOPIC << sentence;
		writeCardView(board.getDimensions().vLine, board.getDimensions().hColumn, toWrite);
	}

	for (;;) { //ask name
		paddingAndTopic(WHITE, true); std::cout << "Player " << position << " name: ";
		std::getline(std::cin, name); cleanBuffer();
		stripSpaces(name);

		if (name == "random") {
			paddingAndTopic(RED, true); std::cout << "That is a reserved keyword. Please choose a different name.\n";
			continue;
		}

		upperNameInitials(name);

		if (name == "") {
			paddingAndTopic(RED, true); std::cout << "We do not accept empty names!\n";
		}
		else if (name.size() > MAX_PLAYER_NAME_SIZE) {
			paddingAndTopic(RED, true); std::cout << "Please do not input large names!\n";
		}
		else if (!isAlpha(name,true)) {
			paddingAndTopic(RED, true); std::cout << "Please do not use digits or special characters.\n";
		}
		else if (std::find(forbiddenNames.begin(), forbiddenNames.end(), name) != forbiddenNames.end()) {
			paddingAndTopic(RED, true); std::cout << "Another player has already chosen that name. Try again.\n";
		}
		else if (name.find("Computer") != std::string::npos || name.find("Bot") != std::string::npos) {
			isBot = true;
			paddingAndTopic(BLUE, true); std::cout << "This player will be a bot.\n";
			break;
		}
		else break;
	}


	{
		std::stringstream toWrite;
		std::vector<int> colors = { RED,GREEN,BLUE,PINK,ORANGE };
		std::vector<std::string> colorNames = { "RED","GREEN","BLUE","PINK","ORANGE" };

		toWrite << TOPIC << colors.size() - forbiddenColors.size() << " colors available\n\n";

		for (size_t i = 0; i < colors.size(); ++i) {
			if (std::find(forbiddenColors.begin(), forbiddenColors.end(), colors.at(i)) != forbiddenColors.end()) continue;
			outputForeColor(toWrite, colors.at(i), TOPIC);
			toWrite << ARROW << SPACE << colorNames.at(i) << "\n";
		}
		writeCardView(board.getDimensions().vLine, board.getDimensions().hColumn, toWrite);
	}


	for (;;) { //ask color

		paddingAndTopic(WHITE, true); std::cout << "Player " << position << " color: ";
		std::getline(std::cin, colorName); cleanBuffer();
		stripSpecialChars(colorName); stripSpaces(colorName);
		for (auto& i : colorName) i = static_cast<char>(tolower(i));

		if (colorName == "red") color = RED;
		else if (colorName == "green") color = GREEN;
		else if (colorName == "blue") color = BLUE;
		else if (colorName == "pink") color = PINK;
		else if (colorName == "orange") color = ORANGE;
		else {
			paddingAndTopic(RED, true); std::cout << "We did not recognize that color. Please try again.\n";
			continue;
		}

		if (std::find(forbiddenColors.begin(), forbiddenColors.end(), color) != forbiddenColors.end()) {
			paddingAndTopic(RED, true); std::cout << "Another player has already chosen that color. Try again.\n";
		}
		else break;
	}

	return { name,color, isBot };
}

int askNumberOfPlayers() {
	std::string input; std::string errorMessage; int intInput = 0;

	for (;;) {
		paddingAndTopic(WHITE, true); std::cout << "Number of players: ";
		std::getline(std::cin, input); cleanBuffer();
		stripSpecialChars(input,true); stripSpaces(input);

		if (!isDigit(input)) {
			errorMessage = "Please input a valid number.";
		}
		else {
			intInput = std::stoi(input);
			if (intInput < 2 || intInput > 4) {
				errorMessage = "This game can only be played by 2 to 4 players.";
			}
			else break;
		}

		paddingAndTopic(RED, true); std::cout << errorMessage << "\n";
	}

	return intInput;
}

void printIntro(const Board& introBoard) {
	introBoard.show();

	std::vector<std::string> sentences = {
	"Welcome to our Scrabble Junior Game!",
	"We hope you'll have a great time with us.",
	"Press enter to create a new game!",
	};

	for (auto& sentence : sentences) {
		paddingAndTopic(BLUE, true); std::cout << sentence;
	}

	std::cout << "\n"; askEnter();
}

int main()
{
	setupConsole();

	auto clearAndShowBoard = [](Board& board) {
		clearConsole();
		board.show();
	};

	Board introBoard("intro_board.txt");

	int nPlayers;
	std::vector<std::string> playerNames;
	std::vector<int> playerColors;
	std::vector<bool> botFlags;

	printIntro(introBoard); clearAndShowBoard(introBoard);

	std::string filename = askBoardFileName(introBoard);
	Board gameBoard(filename);

	clearAndShowBoard(gameBoard); nPlayers = askNumberOfPlayers();

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

	for (;;) {
		my_game.askCommand(1);
		if (my_game.hasFinished()) break;
		my_game.askCommand(2);
		if (my_game.hasFinished()) break;
		my_game.nextTurn();
	};

	my_game.showEndMessage();
}