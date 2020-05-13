#include "Player.h"
#include "../common/Board.h"
#include "Command.h"
#include "../common/ConsoleSetup.h"
#include "Move.h"
#include "Game.h"
#include "Pool.h"
#include "../common/StringProcess.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>

//for shuffle purposes
unsigned const SEED = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 RANDOM_GENERATOR(SEED);

struct PlayerData {
	std::string name;
	int color;
};

int askPlayFirst(const Board* board, int nPlayers, std::vector<std::string> playerNames, std::vector<int> playerColors) {

	std::function<void(int, int)> write = [&](int line, int col) {
		std::vector<std::string> sentences = {
			"|Choose one of the players",
			"|or type 'random' to randomize.",
			" " };

		for (auto& i : sentences) {
			std::cout << i; putCursorOnPos(line++, col);
		}

		for (int i = 0; i < nPlayers; ++i) {
			putCursorOnPos(line++, col);
			printForeColor(playerColors.at(i), '|');
			std::cout << "-> " << playerNames.at(i);
		}
	};

	writeCardView(board->getDimensions().vLine, board->getDimensions().hCollumn, write);

	std::string playerName;
	for (;;) {
		paddingAndTopic(WHITE,true); std::cout << "Who should go first? ";
		std::getline(std::cin, playerName); cleanBuffer();
		playerName = stripSpaces(playerName);

		if (playerName == "random") {
			std::uniform_int_distribution<int> distribution{ 0, nPlayers - 1 };
			int randomPos = distribution(RANDOM_GENERATOR);

			paddingAndTopic(playerColors.at(randomPos), true); std::cout << playerNames.at(randomPos) << " won the draw.\n";
			paddingAndTopic(playerColors.at(randomPos), false); std::cout << "Press enter to start!\n";
			askEnter();
			return randomPos;
		}

		playerName = upperNameInitials(playerName);

		for (int i = 0; i < nPlayers; ++i) {
			if (playerName == playerNames.at(i)) return i;
		}

		paddingAndTopic(RED,true); std::cout << "We could not find a player with that exact name. Please try again.\n";
	}

	return 0;
}

bool exists(std::string filename) {
	std::ifstream file(filename);
	return file.is_open();
}

std::string askBoardFileName(const Board* board) {

	std::function<void(int, int)> write = [&](int line, int col) {
		std::vector<std::string> sentences = {
			"|Tip",
			" ",
			"|You can use our companion",
			"|Board Builder program",
			"|to create your own board!"
		};

		for (std::string str : sentences) {
			std::cout << str; putCursorOnPos(line++, col);
		}
	};

	writeCardView(board->getDimensions().vLine, board->getDimensions().hCollumn, write);
	std::string fileName;

	for (;;) {
		paddingAndTopic(WHITE,true); std::cout << "Import board from file: ";
		std::getline(std::cin, fileName); cleanBuffer();

		if (fileName == "") {
			paddingAndTopic(RED,true); std::cout << "Empty filenames are invalid. Please try again.\n";
		}
		else if (!exists(fileName)) {
			paddingAndTopic(RED,true); std::cout << "We could not find that file. Please try again.\n";
		}
		else {
			Board testBoard(fileName);
			if (testBoard.getNonEmptyChars().size() < MAX_BOARD_LETTERS_WARNING) {
				std::string userAns;

				for (;;) {
					paddingAndTopic(WHITE, true); std::cout << "That board hasn't got enough letters to create a fair 4 player game.\n";
					paddingAndTopic(WHITE, false); std::cout << "Are you sure you want to proceed? ";
					std::getline(std::cin, userAns); cleanBuffer();
					userAns = stripSpaces(userAns);
					for (auto& i : userAns) i = tolower(i);

					if (userAns == "yes" || userAns == "y") return fileName;
					if (userAns == "no" || userAns == "n") break;
					else {
						paddingAndTopic(RED, true); std::cout << "Please answer 'yes' or 'no'.\n";
						continue;
					}
				}
			}

			else break; //board ok
		}
	}

	return fileName;
}

PlayerData askPlayer(int position, const Board *board, std::vector<std::string> forbiddenNames, std::vector<int> forbiddenColors) {

	std::string name, colorName;
	int color = WHITE;
	position++;

	std::function<void(int, int)> write = [&](int line, int col) {
		std::vector<int> colors = { RED,GREEN,BLUE,PINK,ORANGE };
		std::vector<std::string> colorNames = { "RED","GREEN","BLUE","PINK","ORANGE" };

		std::cout << "|" << colors.size() - forbiddenColors.size() << " colors available";
		line++;

		for (size_t i = 0; i < colors.size(); ++i) {
			if (std::find(forbiddenColors.begin(), forbiddenColors.end(), colors.at(i)) != forbiddenColors.end()) continue;
			putCursorOnPos(line++, col);
			printForeColor(colors.at(i), TOPIC);
			std::cout << "-> " << colorNames.at(i);
		}
	};


	for (;;) { //ask name
		paddingAndTopic(WHITE, true); std::cout << "Player " << position << " name: ";
		std::getline(std::cin, name); cleanBuffer();
		name = stripSpaces(name);

		if (name == "random") {
			paddingAndTopic(RED, true); std::cout << "That is a reserved keyword. Please choose a different name.\n";
			continue;
		}

		name = upperNameInitials(name);

		if (name == "") {
			paddingAndTopic(RED,true); std::cout << "We do not accept empty names!\n";
		}
		else if (name.size() > MAX_PLAYER_NAME_SIZE) {
			paddingAndTopic(RED,true); std::cout << "Please do not input large names!\n";
		}
		else if (!isAlpha(name)) {
			paddingAndTopic(RED, true); std::cout << "Please do not use digits or special characters.\n";
		}
		else if (std::find(forbiddenNames.begin(), forbiddenNames.end(), name) != forbiddenNames.end()) {
			paddingAndTopic(RED,true); std::cout << "Another player has already chosen that name. Try again.\n";
		}
		else break;
	}

	writeCardView(board->getDimensions().vLine, board->getDimensions().hCollumn, write);

	for (;;) { //ask color

		paddingAndTopic(WHITE,true); std::cout << "Player " << position << " color: ";
		std::getline(std::cin, colorName); cleanBuffer();
		colorName = stripSpecialChars(colorName); colorName = stripSpaces(colorName);
		for (auto& i : colorName) i = tolower(i);

		if (colorName == "red") color = RED;
		else if (colorName == "green") color = GREEN;
		else if (colorName == "blue") color = BLUE;
		else if (colorName == "pink") color = PINK;
		else if (colorName == "orange") color = ORANGE;
		else {
			paddingAndTopic(RED,true); std::cout << "We did not recognize that color. Please try again.\n";
			continue;
		}

		if (std::find(forbiddenColors.begin(), forbiddenColors.end(), color) != forbiddenColors.end()) {
			paddingAndTopic(RED,true); std::cout << "Another player has already chosen that color. Try again.\n";
		}
		else break;
	}

	return { name,color };
}

int askNumberOfPlayers(const Board* board) {
	std::string input; std::string errorMessage; int intInput;

	for (;;) {
		paddingAndTopic(WHITE, true); std::cout << "Number of players: ";
		std::getline(std::cin, input); cleanBuffer();

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

void printIntro(Board* introBoard) {
	introBoard->show();

	std::vector<std::string> sentences = {
	"Welcome to our Scrabble Junior Game!",
	"We hope you'll have a great time with us.",
	"Press enter to create a new game!",
	};

	for (auto &sentence : sentences) {
		paddingAndTopic(BLUE, true); std::cout << sentence;
	}

	std::cout << "\n"; askEnter();
}

int main()
{
	setupConsole();

	auto clearAndShowBoard = [](const Board& board) {
		clearConsole();
		board.show();
	};

	Board introBoard;

	if (!exists("intro_board.txt")) {
		introBoard = Board();
	}
	else {
		introBoard = Board("intro_board.txt");
	}

	int nPlayers = 2; std::vector<std::string> playerNames; std::vector<int> playerColors;

	printIntro(&introBoard); clearAndShowBoard(introBoard);

	std::string filename = askBoardFileName(&introBoard);
	Board gameBoard(filename);
	clearAndShowBoard(gameBoard);

	nPlayers = askNumberOfPlayers(&gameBoard);

	for (int i = 0; i < nPlayers; ++i) {
		clearAndShowBoard(gameBoard);
		PlayerData player = askPlayer(i, &gameBoard, playerNames, playerColors);
		playerNames.push_back(player.name); playerColors.push_back(player.color);
	}

	clearAndShowBoard(gameBoard);
	int first = askPlayFirst(&gameBoard, nPlayers, playerNames, playerColors);
	Game my_game(&gameBoard, playerNames, playerColors, first);

	do {
		for (int moveNumber : {1, 2}) my_game.askCommand(moveNumber);
		my_game.nextTurn();
	} while (!my_game.hasFinished());

	my_game.end();
}