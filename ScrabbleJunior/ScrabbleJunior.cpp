#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>
#include "Pool.h"
#include "Player.h"
#include "../common/Board.h"
#include "Command.h"
#include "../common/ConsoleSetup.h"
#include "Move.h"
#include "Game.h"
#include "../common/StringProcess.h"

//for shuffle purposes
unsigned const SEED = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 RANDOM_GENERATOR(SEED);

struct PlayerData {
	std::string name;
	int color;
};

void getReady() {
	paddingAndTopic(WHITE,true); std::cout << "Everything set!\n";
	paddingAndTopic(WHITE,true); std::cout << "Press enter to start!\n";
	askEnter();
}

int askPlayFirst(const Board* board, int nPlayers, std::vector<std::string> playerNames, std::vector<int> playerColors,
	std::function<void(const Board*)> clear) {

	std::function<void(int, int)> write = [&](int line, int col) {
		std::cout << "|Choose one of the players";
		putCursorOnPos(line++, col);
		std::cout << "|or type 'random' to randomize.";
		line++;

		for (int i = 0; i < nPlayers; ++i) {
			putCursorOnPos(line++, col);
			printForeColor(playerColors.at(i), '|');
			std::cout << "-> " << playerNames.at(i);
		}
	};

	clear(board);
	writeCardView(board->getDimensions().vLine, board->getDimensions().hCollumn, write);

	std::string playerName;
	for (;;) {
		paddingAndTopic(WHITE,true); std::cout << "Who should go first? ";
		std::getline(std::cin, playerName); cleanBuffer();
		playerName = stripSpaces(playerName);

		if (playerName == "clear") {
			clear(board); writeCardView(board->getDimensions().vLine, board->getDimensions().hCollumn, write);
			continue;
		}

		if (playerName == "random") {
			std::uniform_int_distribution<int> distribution{ 0, nPlayers - 1 };
			int randomPos = distribution(RANDOM_GENERATOR);
			paddingAndTopic(playerColors.at(randomPos), true); std::cout << playerNames.at(randomPos) << " won the draw.\n";
			paddingAndTopic(playerColors.at(randomPos), false); std::cout << "Press enter to continue.\n";
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
	return !(!file);
}

std::string askBoardFileName(const Board* board, std::function<void(const Board*)> clear) {

	std::function<void(int, int)> write = [&](int line, int col) {
		std::cout << "|Tip";
		line++; putCursorOnPos(line++, col);
		std::cout << "|You can use our companion";
		putCursorOnPos(line++, col);
		std::cout << "|Board Builder program";
		putCursorOnPos(line++, col);
		std::cout << "|to create your own board!";
	};

	clear(board);
	writeCardView(board->getDimensions().vLine, board->getDimensions().hCollumn, write);
	std::string fileName;

	for (;;) {
		paddingAndTopic(WHITE,true); std::cout << "Import board from file: ";

		std::getline(std::cin, fileName); cleanBuffer();

		if (fileName == "clear") {
			clear(board); writeCardView(board->getDimensions().vLine, board->getDimensions().hCollumn, write);
			continue;
		}

		if (fileName == "") {
			paddingAndTopic(RED,true); std::cout << "Empty filenames are invalid. Please try again.\n";
		}
		else if (!exists(fileName)) {
			paddingAndTopic(RED,true); std::cout << "We could not find that file. Please try again.\n";
		}
		else {
			Board testBoard(fileName);
			if (testBoard.getNonEmptyChars().size() < 7 * 4) {
				std::string userAns;

				for (;;) {
					paddingAndTopic(WHITE, true); std::cout << "That board hasn't got enough letters to create a fair 4 player game.\n";
					paddingAndTopic(WHITE, false); std::cout << "Are you sure you want to proceed? ";
					std::getline(std::cin, userAns); cleanBuffer();
					userAns = stripSpaces(userAns);

					if (userAns == "clear") {
						clear(board); writeCardView(board->getDimensions().vLine, board->getDimensions().hCollumn, write);
						continue;
					}

					for (auto& i : userAns)i = tolower(i);

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

PlayerData askPlayer(int position, const Board *board,
	std::vector<std::string> forbiddenNames, std::vector<int> forbiddenColors,
	std::function<void(const Board*)> clear) {

	std::string name, colorName;
	int color = WHITE;
	position++;
	clear(board);

	std::function<void(int, int)> write = [&](int line, int col) {
		std::vector<int> colors = { RED,GREEN,BLUE,PINK,ORANGE };
		std::vector<std::string> colorNames = { "RED","GREEN","BLUE","PINK","ORANGE" };

		std::cout << "|" << colors.size() - forbiddenColors.size() << " colors available";
		line++;

		for (int i = 0; i < (int)colors.size(); ++i) {
			if (std::find(forbiddenColors.begin(), forbiddenColors.end(), colors.at(i)) != forbiddenColors.end()) continue;
			putCursorOnPos(line++, col);
			printForeColor(colors.at(i), TOPIC);
			std::cout << "-> " << colorNames.at(i) << "\n";
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
		else if (name == "clear") {
			clear(board);
			continue;
		}

		name = upperNameInitials(name);

		if (name == "") {
			paddingAndTopic(RED,true); std::cout << "We do not accept empty names!\n";
		}
		else if (name.size() > 20) {
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
		colorName = stripSpecialChars(colorName);
		colorName = stripSpaces(colorName);
		for (auto& i : colorName) i = tolower(i);

		if (colorName == "clear") {
			clear(board); writeCardView(board->getDimensions().vLine, board->getDimensions().hCollumn, write);
			continue;
		}

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

int askNumberOfPlayers(const Board* board, std::function<void(const Board*)> clear) {
	clear(board);
	std::string input;

	for (;;) {
		paddingAndTopic(WHITE, true); std::cout << "Number of players: ";
		std::getline(std::cin, input); cleanBuffer();
		input = stripSpaces(input);

		if (input == "clear") {
			clear(board);
			continue;
		}

		else {
			if (!isDigit(input) || std::stoi(input) > 4 || std::stoi(input) < 2) {
				paddingAndTopic(RED, true);
				std::cout << "Please input a number between 2 and 4.\n";
			}
			else break;
		}
	}

	return std::stoi(input);
}

void printIntro(Board* introBoard) {
	introBoard->show();

	std::vector<std::string> sentences = {
	"Welcome to our Scrabble Junior Game!",
	"We hope you'll have a great time with us.",
	"\n",
	"Throughout the game, type 'help' to view the available commands.",
	"Type 'clear' to erase command history and reload in case of issues."
	"\n",
	"Press enter to create a new game!",
	"\n"
	};

	for (auto sentence : sentences) {
		if (sentence != "\n") paddingAndTopic(WHITE, true);
		std::cout << sentence;
	}

	askEnter();
}

int main()
{
	setupConsole();

	auto clearAndShowBoard = [](const Board* board) {
		clearConsole();
		board->show();
	};

	while (!exists("intro_board.txt")) {
		std::cerr << "Could not find intro_board.txt in the project folder!";
	    std::cout << "\nThis file is necessary to load the program introductory board.\n";
		std::cout << "Fix this issue and press enter to try again.\n";
		askEnter();
	}

	Board introBoard("intro_board.txt");
	int nPlayers = 2;
	std::vector<std::string> playerNames;
	std::vector<int> playerColors;

	printIntro(&introBoard);

	std::string filename = askBoardFileName(&introBoard, clearAndShowBoard);
	Board gameBoard(filename);

	nPlayers = askNumberOfPlayers(&gameBoard, clearAndShowBoard);

	for (int i = 0; i < nPlayers; ++i) {
		PlayerData player = askPlayer(i, &gameBoard, playerNames, playerColors, clearAndShowBoard);
		playerNames.push_back(player.name);
		playerColors.push_back(player.color);
	}

	int first = askPlayFirst(&gameBoard, nPlayers, playerNames, playerColors, clearAndShowBoard);

	Game my_game(&gameBoard, playerNames, playerColors, first);

	clearAndShowBoard(&gameBoard);
	getReady();

	for (;;) {
		my_game.askCommand(1);
		if (my_game.hasFinished()) break;
		my_game.askCommand(2);
		if (my_game.hasFinished()) break;
		my_game.nextTurn();
	};

	my_game.end();
}