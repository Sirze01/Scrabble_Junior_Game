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

int askPlayFirst(int boardWidth, int boardHeight, int nPlayers, std::vector<std::string> playerNames, std::vector<int> playerColors) {

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

	writeCardView(boardHeight, boardWidth, write);

	std::string playerName;
	for (;;) {
		paddingAndTopic(WHITE,true); std::cout << "Who should go first? ";
		std::getline(std::cin, playerName); cleanBuffer();
		playerName = stripSpaces(playerName);

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

std::string askBoardFileName(int boardWidth, int boardHeight) {

	std::function<void(int, int)> write = [&](int line, int col) {
		std::cout << "|Tip";
		line++; putCursorOnPos(line++, col);
		std::cout << "|You can use our companion";
		putCursorOnPos(line++, col);
		std::cout << "|Board Builder program";
		putCursorOnPos(line++, col);
		std::cout << "|to create your own board!";
	};


	writeCardView(boardHeight, boardWidth, write);
	std::string filename;

	for (;;) {
		paddingAndTopic(WHITE,true); std::cout << "Import board from file: ";

		std::getline(std::cin, filename); cleanBuffer();
		if (filename == "") {
			paddingAndTopic(RED,true); std::cout << "Empty filenames are invalid. Please try again.\n";
		}
		else if (!exists(filename)) {
			paddingAndTopic(RED,true); std::cout << "We could not find that file. Please try again.\n";
		}
		else {
			Board testBoard(filename);
			if (testBoard.getNonEmptyChars().size() < 7 * 4) {
				std::string userAns;
				paddingAndTopic(RED, true); std::cout << "That board hasn't got enough letters to create a fair 4 player game.\n";
				paddingAndTopic(RED, false); std::cout << "Are you sure you want to proceed? ";

				for (;;) {
					std::getline(std::cin, userAns); cleanBuffer();
					userAns = stripSpaces(userAns);
					for (auto& i : userAns)i = tolower(i);

					if (userAns == "yes" || userAns == "y") return filename;
					if (userAns == "no" || userAns == "n") break;
					else {
						paddingAndTopic(RED, true); std::cout << "Please answer 'yes' or 'no': ";
						continue;
					}
				}
			}

			else break; //board ok
		}
	}

	return filename;
}

PlayerData askPlayer(int position, int boardWidth, int boardHeight, std::vector<std::string> forbiddenNames, std::vector<int> forbiddenColors) {
	std::string name, colorName;
	int color = WHITE;
	position++;

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
			paddingAndTopic(RED, true); std::cout << "We might use that keyword later. Please choose a different name.\n";
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

	writeCardView(boardHeight, boardWidth, write);

	for (;;) { //ask color

		paddingAndTopic(WHITE,true); std::cout << "Player " << position << " color: ";
		std::getline(std::cin, colorName); cleanBuffer();
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

	askEnter();
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

	askEnter();
}

int main()
{
	setupConsole();

	auto clearAndShowBoard = [](Board* board) {
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

	clearAndShowBoard(&introBoard);
	std::string filename = askBoardFileName(introBoard.getDimensions().hCollumn, introBoard.getDimensions().vLine);
	Board gameBoard(filename);

	clearAndShowBoard(&gameBoard);
	nPlayers = askNumberOfPlayers();

	for (int i = 0; i < nPlayers; ++i) {
		clearAndShowBoard(&gameBoard);
		PlayerData player = askPlayer(i, gameBoard.getDimensions().hCollumn, gameBoard.getDimensions().vLine, playerNames, playerColors);
		playerNames.push_back(player.name);
		playerColors.push_back(player.color);
	}

	clearAndShowBoard(&gameBoard);
	int first = askPlayFirst(gameBoard.getDimensions().hCollumn, gameBoard.getDimensions().vLine, nPlayers, playerNames, playerColors);

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