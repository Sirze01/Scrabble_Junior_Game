#include <iostream>
#include <string>
#include <algorithm>
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

void endgame(Game* game) {
    game->showScores();
    if (game->hasWinner()) {
        std::cout << LEFT_PADDING_STR << game->getPlayerName(game->getWinner()) << " won!\n";
    }
    else std::cout << LEFT_PADDING_STR << "There has been a draw! Congrats to all.\n";
    std::cin.ignore(10000, '\n');
}

PlayerData askPlayer(int position, int introBoardWidth, std::vector<std::string> forbiddenNames, std::vector<int> forbiddenColors) {
    PlayerData player;
    std::string name = "Default";
    std::string colorName;
    int color = WHITE;
    position++;

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

        for (int i = 0; i < (int) colors.size(); ++i) {
            if (std::find(forbiddenColors.begin(),forbiddenColors.end(),colors.at(i)) != forbiddenColors.end()) continue;
            putCursorOnPos(line++, col);
            printForeColor(colors.at(i), '|');
            std::cout << "-> " << colorNames.at(i) << "\n";
        }

        restoreSavedCursorPosition();
    };


    for (;;) { //ask name
        std::cout << LEFT_PADDING_STR << "|Player " << position << " name: ";
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
        colorName = stripSpaces(colorName);
        for (auto& i : colorName) i = tolower(i);
        if (colorName == "red") {
            color= RED;
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
            std::cout << "Please input a number between 2 and 4!\n\n";
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
    "|Press enter to create a new game!"
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

    int nPlayers = 2;
    Board intro_board("intro_board_debug.txt");

    auto clearAndShowIntroBoard = [&]() { //[&] means capture everything outside this scope
        clearConsole();
        intro_board.show();
    };


    //DEBUG
    std::vector<std::string> playerNames = { "Nunca Sentado", "Comboios" };
    std::vector<int> playerColors = { RED, PINK };

    /*
    std::vector<std::string> playerNames, forbiddenNames;
    std::vector<int> playerColors, forbiddenColors;
    
    intro_board.show();
    printIntro();
    clearAndShowIntroBoard();

    nPlayers = askNumberOfPlayers();
    clearAndShowIntroBoard();

    for (int i = 0; i < nPlayers; ++i) {
        clearAndShowIntroBoard();
        PlayerData player = askPlayer(i, 17, forbiddenNames, forbiddenColors);

        playerNames.push_back(player.name);
        forbiddenNames.push_back(player.name);

        playerColors.push_back(player.color);
        forbiddenColors.push_back(player.color);
    }
    */

    Game my_game(&intro_board, playerNames, playerColors, 0);

    for (;;) {
        my_game.askCommand(1);
        my_game.askCommand(2);
        my_game.nextTurn();
        if (my_game.hasFinished()) break;
    }

    clearConsole();
    intro_board.show();
    endgame(&my_game);
}