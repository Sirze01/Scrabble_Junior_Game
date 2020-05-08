#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "Pool.h"
#include "Player.h"
#include "../common/Board.h"
#include "Command.h"
#include "../common/ConsoleSetup.h"
#include "Move.h"
#include "Game.h"
#include "../common/StringProcess.h"

struct PlayerData {
	std::string name;
	int color;
};

void getReady() {
	paddingAndTopic(WHITE,true); std::cout << "Everything set!\n";
	paddingAndTopic(WHITE,true); std::cout << "Press enter to start!\n";
	std::cin.ignore(10000, '\n');
}

int askPlayFirst(int BoardWidth, int nPlayers, std::vector<std::string> playerNames, std::vector<int> playerColors) {
	auto showGamePlayers = [&]() {
		saveCurrentCursorPosition();

		int line = 4 + BOARD_TOP_PADDING;
		int col = 1 + 2 * BoardWidth + CARD_LEFT_PADDING;

		putCursorOnPos(line++, col);
		std::cout << "|Players";
		line++;

		for (int i = 0; i < nPlayers; ++i) {
			putCursorOnPos(line++, col);
			printForeColor(playerColors.at(i), '|');
			std::cout << "-> " << playerNames.at(i);
		}

		restoreSavedCursorPosition();
	};

	showGamePlayers();

	std::string playerName;
	for (;;) {
		paddingAndTopic(WHITE,true); std::cout << "Who should go first? ";
		std::getline(std::cin, playerName);
		playerName = stripSpaces(playerName);
		playerName = upperNameInitials(playerName);

		for (int i = 0; i < nPlayers; ++i) {
			if (playerName == playerNames.at(i)) return i;
		}

		paddingAndTopic(RED,true); std::cout << "We could not find a player with that name. Please try again.\n";
	}

	return 0;
}

bool exists(std::string filename) {
	std::ifstream file(filename);
	return !(!file);
}

std::string askBoardFileName(int BoardWidth) {
	auto showCompanionAppTip = [&]() {
		saveCurrentCursorPosition();

		int line = 4 + BOARD_TOP_PADDING;
		int col = 1 + 2 * BoardWidth + CARD_LEFT_PADDING;

		putCursorOnPos(line++, col);
		std::cout << "|Tip";
		line++; putCursorOnPos(line++, col);
		std::cout << "|You can use our companion";
		putCursorOnPos(line++, col);
		std::cout << "|Board Builder program";
		putCursorOnPos(line++, col);
		std::cout << "|to create your own board!";

		restoreSavedCursorPosition();
	};

	showCompanionAppTip();
	std::string filename;

	for (;;) {
		paddingAndTopic(WHITE,true); std::cout << "Import board from file: ";

		std::getline(std::cin, filename);
		if (filename == "") {
			paddingAndTopic(RED,true); std::cout << "Empty filenames are invalid. Please try again.\n";
		}
		else if (!exists(filename)) {
			paddingAndTopic(RED,true); std::cout << "We could not find that file. Please try again.\n";
		}
		else break;
	}

	return filename;
}

PlayerData askPlayer(int position, int introBoardWidth, std::vector<std::string> forbiddenNames, std::vector<int> forbiddenColors) {
	std::string name, colorName;
	int color = WHITE;

	auto showAvailableColors = [&]() {
		saveCurrentCursorPosition();

		int line = 3 + BOARD_TOP_PADDING;
		int col = 1 + 2 * introBoardWidth + CARD_LEFT_PADDING;

		std::vector<int> colors = { RED,GREEN,BLUE,PINK,ORANGE };
		std::vector<std::string> colorNames = { "RED","GREEN","BLUE","PINK","ORANGE" };

		putCursorOnPos(line++, col);
		std::cout << "|" << colors.size() - forbiddenColors.size() << " colors available";
		line++;

		for (int i = 0; i < (int)colors.size(); ++i) {
			if (std::find(forbiddenColors.begin(), forbiddenColors.end(), colors.at(i)) != forbiddenColors.end()) continue;
			putCursorOnPos(line++, col);
			printForeColor(colors.at(i), TOPIC);
			std::cout << "-> " << colorNames.at(i) << "\n";
		}

		restoreSavedCursorPosition();
	};


	for (;;) { //ask name
		paddingAndTopic(WHITE, true); std::cout << "Player " << ++position << " name: ";
		std::getline(std::cin, name);
		name = stripSpaces(name);
		name = upperNameInitials(name);

		if (name == "") {
			paddingAndTopic(RED,true); std::cout << "We do not accept empty names!\n";
		}
		else if (name.size() > 20) {
			paddingAndTopic(RED,true); std::cout << "Please do not input large names!\n";
		}
		else if (std::find(forbiddenNames.begin(), forbiddenNames.end(), name) != forbiddenNames.end()) {
			paddingAndTopic(RED,true); std::cout << "Another player has already chosen that name. Try again.\n";
		}
		else break;
	}

	for (;;) { //ask color
		showAvailableColors();
		paddingAndTopic(WHITE,true); std::cout << "Player " << position << " color: ";
		std::getline(std::cin, colorName);
		colorName = stripSpecialChars(colorName);
		colorName = stripSpaces(colorName);
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

int askNumberOfPlayers() {
	int nPlayers = 2;

	for (;;) {
		paddingAndTopic(WHITE,true); std::cout << "Number of players: ";
		if (!(std::cin >> nPlayers) || nPlayers > 4 || nPlayers < 2) {
			cleanBuffer();
			paddingAndTopic(RED, true);
			std::cout << "Please input a number between 2 and 4.\n";
		}
		else break;
	}

	std::cin.ignore(10000, '\n');
	return nPlayers;
}

void printIntro(Board* introBoard) {
	introBoard->show();

	std::vector<std::string> sentences = {
	"Welcome to our Scrabble Junior Game!",
	"We hope you'll have a great time with us.",
	"\n",
	"Throughout the game, type 'help' to view the available commands.",
	"\n",
	"Press enter to create a new game!",
	"\n"
	};

	for (auto sentence : sentences) {
		if (sentence != "\n") paddingAndTopic(WHITE, true);
		std::cout << sentence;
	}

	std::cin.ignore(10000, '\n');
}

int main()
{
	setupConsole();

	auto clearAndShowBoard = [](Board* board) {
		clearConsole();
		board->show();
	};

	if (!exists("intro_board.txt")) {
		std::cerr << "Fatal error - could not find intro_board.txt! Check your project folder...";
		return 1;
	}

	Board introBoard("intro_board.txt");
	int nPlayers = 2;
	std::vector<std::string> playerNames;
	std::vector<int> playerColors;

	printIntro(&introBoard);

	clearAndShowBoard(&introBoard);
	std::string filename = askBoardFileName(introBoard.getDimensions().hCollumn);
	Board gameBoard(filename);

	clearAndShowBoard(&gameBoard);
	nPlayers = askNumberOfPlayers();

	for (int i = 0; i < nPlayers; ++i) {
		clearAndShowBoard(&gameBoard);
		PlayerData player = askPlayer(i, gameBoard.getDimensions().hCollumn, playerNames, playerColors);
		playerNames.push_back(player.name);
		playerColors.push_back(player.color);
	}

	clearAndShowBoard(&gameBoard);
	int first = askPlayFirst(gameBoard.getDimensions().hCollumn, nPlayers, playerNames, playerColors);

	Game my_game(&gameBoard, playerNames, playerColors, first);

	clearAndShowBoard(&gameBoard);
	getReady();

	do {
		my_game.askCommand(1);
		my_game.askCommand(2);
		my_game.nextTurn();
	} while (!my_game.hasFinished());

	my_game.end();
}