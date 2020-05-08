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
	std::cout << LEFT_PADDING_STR << "|Everything set!\n\n";
	std::cout << LEFT_PADDING_STR << "|Press enter to start! ";
	std::cin.ignore(10000, '\n');
}

int askPlayFirst(int BoardWidth, int nPlayers, std::vector<std::string> playerNames, std::vector<int> playerColors) {
	auto paddingAndRedUpChar = []() { //[&] means capture everything outside this scope
		std::cout << "\n" << LEFT_PADDING_STR;
		printForeColor(RED, '|');
	};

	auto showGamePlayers = [&]() {
		saveCurrentCursorPosition();

		int line = 3 + BOARD_TOP_PADDING;
		int col = 1 + 2 * BoardWidth + CARD_LEFT_PADDING;

		putCursorOnPos(line++, col);
		std::cout << "|Players";
		putCursorOnPos(line++, col);
		std::cout << "\n";

		for (int i = 0; i < nPlayers; ++i) {
			putCursorOnPos(line++, col);
			printForeColor(playerColors.at(i), '|');
			std::cout << "-> " << playerNames.at(i);
			std::cout << "\n\n";
		}

		restoreSavedCursorPosition();
	};
	showGamePlayers();

	std::string playerName;
	for (;;) {
		std::cout << LEFT_PADDING_STR << "|Who should go first? ";
		std::getline(std::cin, playerName);
		playerName = stripSpaces(playerName);

		for (int i = 0; i < nPlayers; ++i) {
			if (playerName == playerNames.at(i)) return i;
		}

		paddingAndRedUpChar();
		std::cout << "We could not find a player with that name. Please try again.\n\n";
	}

	return 0;
}

bool exists(std::string filename) {
	std::ifstream file(filename);
	return !(!file);
}

std::string askBoardFileName() {
	std::string filename;
	auto paddingAndRedUpChar = []() { //[&] means capture everything outside this scope
		std::cout << "\n" << LEFT_PADDING_STR;
		printForeColor(RED, '|');
	};

	for (;;) {
		std::cout << LEFT_PADDING_STR << "|Import board from file: ";
		std::getline(std::cin, filename);

		if (filename == "") {
			paddingAndRedUpChar();
			std::cout << "Empty filenames are invalid. Please try again.\n\n";
		}
		else if (!exists(filename)) {
			paddingAndRedUpChar();
			std::cout << "We could not find that file. Please try again.\n\n";
		}
		else break;
	}

	return filename;
}

PlayerData askPlayer(int position, int introBoardWidth, std::vector<std::string> forbiddenNames, std::vector<int> forbiddenColors) {
	std::string name = "Default";
	std::string colorName;
	int color = WHITE;
	PlayerData player = { name,color };

	auto paddingAndRedUpChar = []() { //[&] means capture everything outside this scope
		std::cout << "\n" << LEFT_PADDING_STR;
		printForeColor(RED, '|');
	};

	auto showAvailableColors = [&]() {
		saveCurrentCursorPosition();

		int line = 3 + BOARD_TOP_PADDING;
		int col = 1 + 2 * introBoardWidth + CARD_LEFT_PADDING;

		std::vector<int> colors = { RED,GREEN,BLUE,PINK,ORANGE };
		std::vector<std::string> colorNames = { "RED","GREEN","BLUE","PINK","ORANGE" };

		putCursorOnPos(line++, col);
		std::cout << "|" << colors.size() - forbiddenColors.size() << " colors available";
		putCursorOnPos(line++, col);
		std::cout << "\n";

		for (int i = 0; i < (int)colors.size(); ++i) {
			if (std::find(forbiddenColors.begin(), forbiddenColors.end(), colors.at(i)) != forbiddenColors.end()) continue;
			putCursorOnPos(line++, col);
			printForeColor(colors.at(i), '|');
			std::cout << "-> " << colorNames.at(i) << "\n";
		}

		restoreSavedCursorPosition();
	};


	for (;;) { //ask name
		std::cout << LEFT_PADDING_STR << "|Player " << ++position << " name: ";
		std::getline(std::cin, name);
		name = stripSpaces(name);

		if (name == "") {
			paddingAndRedUpChar();
			std::cout << "We do not accept empty names!\n\n";
		}
		else if (name.size() > 20) {
			paddingAndRedUpChar();
			std::cout << "Please do not input large names!\n\n";
		}
		else if (std::find(forbiddenNames.begin(), forbiddenNames.end(), name) != forbiddenNames.end()) {
			paddingAndRedUpChar();
			std::cout << "Another player has already chosen that name. Try again.\n\n";
		}
		else break;
	}

	std::cout << std::endl;

	for (;;) { //ask color
		showAvailableColors();
		std::cout << LEFT_PADDING_STR << "|Player " << position << " color: ";
		std::getline(std::cin, colorName);

		colorName = stripSpecialChars(colorName);
		colorName = stripSpaces(colorName);
		for (auto& i : colorName) i = tolower(i);

		if (colorName == "red") {
			color = RED;
		}
		else if (colorName == "green") {
			color = GREEN;
		}
		else if (colorName == "blue") {
			color = BLUE;
		}
		else if (colorName == "pink") {
			color = PINK;
		}
		else if (colorName == "orange") {
			color = ORANGE;
		}
		else {
			paddingAndRedUpChar();
			std::cout << "We did not recognize that color. Please try again.\n\n";
			continue;
		}
		if (std::find(forbiddenColors.begin(), forbiddenColors.end(), color) != forbiddenColors.end()) {
			paddingAndRedUpChar();
			std::cout << "Another player has already chosen that color. Try again.\n\n";
		}
		else break;
	}

	player.name = name;
	player.color = color;

	return player;
}

int askNumberOfPlayers() {
	int nPlayers = 2;

	auto paddingAndRedUpChar = []() { //[&] means capture everything outside this scope
		std::cout << "\n" << LEFT_PADDING_STR;
		printForeColor(RED, '|');
	};

	for (;;) {
		std::cout << LEFT_PADDING_STR << "|Number of players: ";
		if (!(std::cin >> nPlayers) || nPlayers > 4 || nPlayers < 2) {
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			paddingAndRedUpChar();
			std::cout << "Please input a number between 2 and 4.\n\n";
		}
		else break;
	}

	std::cin.ignore(10000, '\n');
	return nPlayers;
}

void printIntro() {
	std::vector<std::string> sentences = {
	"|Welcome to our Scrabble Junior Game!",
	"|We hope you'll have a great time with us.",
	" ",
	"|Throughout the game, type 'help' to view the available commands.",
	" ",
	"|Press enter to create a new game!",
	" "
	};

	for (auto sentence : sentences) {
		std::cout << LEFT_PADDING_STR << sentence << "\n";
	}

	std::cout << LEFT_PADDING_STR;
	std::cin.ignore(10000, '\n');
}

int main()
{
	setupConsole();

	Board intro_board("intro_board.txt");
	int nPlayers = 2;

	auto clearAndShowIntroBoard = [&]() { //[&] means capture everything outside this scope
		clearConsole();
		intro_board.show();
		std::cout << "\n";
	};
	/*
	std::vector<std::string> playerNames;
	std::vector<int> playerColors;

	intro_board.show();
	std::cout << "\n"; printIntro();

	clearAndShowIntroBoard();
	std::string filename = askBoardFileName();
	Board board(filename); //will ask the user in the future

	clearAndShowGameBoard();
	nPlayers = askNumberOfPlayers();

	for (int i = 0; i < nPlayers; ++i) {
		clearAndShowGameBoard();
		PlayerData player = askPlayer(i, board.getDimensions().hCollumn, playerNames, playerColors);

		playerNames.push_back(player.name);
		playerColors.push_back(player.color);
	}

	clearAndShowGameBoard();
	int first = askPlayFirst(board.getDimensions().hCollumn, nPlayers, playerNames, playerColors);

	*/


	//start debug
	Board board("intro_board.txt");
	std::vector<std::string> playerNames = { "Alfredo","Comboios" };
	std::vector<int> playerColors = { RED,GREEN };
	int first = 1;
	//end debug

	auto clearAndShowGameBoard = [&]() {
		clearConsole();
		board.show();
		std::cout << "\n";
	};

	Game my_game(&board, playerNames, playerColors, first);

	clearAndShowGameBoard();
	getReady();

	for (;;) {
		my_game.askCommand(1);
		my_game.askCommand(2);
		my_game.nextTurn();
		if (my_game.hasFinished()) break;
	}

	clearConsole();
	my_game.end();
}