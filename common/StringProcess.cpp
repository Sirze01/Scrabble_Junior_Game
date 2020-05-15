#include "../common/StringProcess.h"
#include "../common/ConsoleSetup.h"
#include <string>
#include <vector>
#include <algorithm>

void lowerCase(std::string &command) {
    for (auto& i : command) i = static_cast<char>(std::tolower(i));
}

void stripSpaces(std::string &name) {
    for (size_t i = 0; i < name.length(); i++) {
        if (name.at(i) == SPACE) {
            if (i == name.length() - 1) {
                name.erase(i, 1);
                break;
            }
            else if (i == 0 || name.at(i + 1) == SPACE) {
                name.erase(i, 1);
                i--;
            }
        }
    }
}

void upperNameInitials(std::string &name) {
    bool doUpper = true;
    for (size_t i = 0; i < name.length(); i++) {
        if (doUpper && isupper(name.at(i))) doUpper = false; //already uppercase
        else if (doUpper && !isupper(name.at(i))) {
            name.at(i) = static_cast<char>(toupper(name.at(i)));
            doUpper = false;
        }
        else if (name.at(i) == SPACE) doUpper = true;
    }
}

void stripSpecialChars(std::string &name, bool acceptDigits) {
    std::string cleanStr;
    for (const char &c : name) {
        if (std::isalpha(c) || c == SPACE || (acceptDigits && std::isdigit(c))) {
            cleanStr.push_back(c);
        }
    }
    name = cleanStr;
}


/**Funtion to write strings across several lines
 *
 * @param text_width - Number of chars per line
 * @param text       - Text to write
 * @param padding    - Spaces before the line
 * */
std::string stringWriter(size_t text_width, std::string text, int padding = 0) {
    size_t charNbr;
    std::string output_str;

    while (!text.empty()) {
        output_str += std::string(padding, SPACE);
        if (text.size() <= text_width) {
            output_str += (text + '\n');
            text.erase(0, text.size());
            continue;
        }

        charNbr = text_width - 1;
        if (text.at(charNbr) == '.' || text.at(charNbr) == ',' || text.at(charNbr) == '!' || text.at(charNbr) == '?' ||
            text.at(charNbr) == ';' || text.at(charNbr) == SPACE) {
            output_str += (text.substr(0, charNbr + 1) + '\n');
            text.erase(0, charNbr + 1);
            continue;
        }


        if (isalnum(charNbr)) {
            if (text.at(charNbr + 1) == SPACE) {
                output_str += (text.substr(0, charNbr + 1) + '\n');
                text.erase(0, charNbr + 1);
                text.erase(0, 1);
                continue;
            } else if (text.at(charNbr + 1) == '.' || text.at(charNbr + 1) == ',' || text.at(charNbr + 1) == '!' ||
                       text.at(charNbr + 1) == '?' || text.at(charNbr + 1) == ';') {
                while (isalnum(text.at(charNbr)) && isalnum(text.at(charNbr - 1))) {
                    charNbr--;
                }
                output_str += (text.substr(0, charNbr) + '\n');
                text.erase(0, charNbr);
                continue;
            } else {
                while (isalnum(text.at(charNbr)) && isalnum(text.at(charNbr - 1))) {
                    charNbr--;
                }
                output_str += (text.substr(0, charNbr) + '\n');
                text.erase(0, charNbr);
                continue;
            }
        }
    }
    return output_str;
}

void stripCommandBloat(std::string &command) {
    std::string cleanCommand(command);
    std::vector<std::string> keywords = //careful not to interfere with board coords or reserved commands
    { "play","move","from","letter","check","get","position","board","load","save","write","show", "tile"};

    for (const std::string &word : keywords) {
        int pos = cleanCommand.find(word);
        if (cleanCommand.find(word) != std::string::npos) {
            cleanCommand.erase(pos, pos + word.size());
        }
    }
}

std::string smartCommandAdvice(const std::string &command) {
    if (command.size() == 2 && isalpha(command.at(0)) && isalpha(command.at(1))) {
        std::string processed;
        processed += static_cast<char>(toupper(command.at(0)));
        processed += static_cast<char>(tolower(command.at(1)));
        return "Did you attempt to play a tile on position " + processed +
               "? Please specify letter as in 'Yx <letter>'.";

    } else if (command.find("exchange") != std::string::npos) {
        return "Are you trying to exchange one of your tiles? Please specify letter as in 'exchange <letter>'.";
    } else {
        return "Command not recognized. Please type 'help' to view the available commands.";
    }
}
    
bool isAlpha(const std::string &toTest, bool acceptSpaces) {
    for (const auto& letter : toTest) {
        if (!std::isalpha(letter)) {
            if (letter == SPACE) {
                if (acceptSpaces) continue;
                else return false;
            }
            else return false;
        }
    }
    return toTest.size();
}

bool isDigit(const std::string &toTest) {
    for (const auto &letter : toTest) if (!std::isdigit(letter)) return false;
    return toTest.size();
}