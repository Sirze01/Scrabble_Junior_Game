#include "../common/StringProcess.h"
#include <string>
#include <vector>
#include <algorithm>

std::string lowerCase(std::string command) {
    for (auto& i : command) i = tolower(i);
    return command;
}

std::string stripSpaces(std::string name) {
    for (size_t i = 0; i < name.length(); i++) {
        if (name.at(i) == ' ') {
            if (i == name.length() - 1) {
                name.erase(i, 1);
                break;
            }
            else if (i == 0 || name.at(i + 1) == ' ') {
                name.erase(i, 1);
                i--;
            }
        }
    }
    return name;
}

std::string stripSpecialChars(std::string name) {
    std::string cleanStr;
    for (char c : name) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ') {
            cleanStr.push_back(c);
        }
    }
    return cleanStr;
}


/**Funtion to write strings across several lines
 *
 * @param text_width - Number of chars per line
 * @param text       - Text to write
 * @param padding    - Spaces before the line
 * */
std::string stringWriter(int text_width, std::string text, int padding = 0) {
    size_t charNbr;
    std::string output_str;

    while (!text.empty()) {
        output_str += std::string(padding, ' ');
        if (text.size() <= text_width) {
            output_str += (text + '\n');
            text.erase(0, text.size());
            continue;
        }

        charNbr = text_width - 1;
        if (text.at(charNbr) == '.' || text.at(charNbr) == ',' || text.at(charNbr) == '!' || text.at(charNbr) == '?' ||
            text.at(charNbr) == ';' || text.at(charNbr) == ' ') {
            output_str += (text.substr(0, charNbr + 1) + '\n');
            text.erase(0, charNbr + 1);
            continue;
        }


        if (isalnum(charNbr)) {
            if (text.at(charNbr + 1) == ' ') {
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

std::string stripCommandBloat(std::string command) {
    //an experiment on smart command interpretation
    std::string cleanCommand(command);
    std::vector<std::string> keywords = //careful not to interfere with board coords or reserved commands
    { "play","move","from","letter","check","get","position","board","load","save","write","show", "tile"};

    for (std::string word : keywords) {
        int pos = cleanCommand.find(word);
        if (cleanCommand.find(word) != std::string::npos) {
            cleanCommand.erase(pos, pos + word.size());
        }
    }
    return cleanCommand;
}

std::string smartCommandAdvice(std::string command) {
    if (command.size() == 2 && isalpha(command.at(0)) && isalpha(command.at(1))) {
        std::string processed;
        processed += toupper(command.at(0));
        processed += tolower(command.at(1));
        return "Did you attempt to play a tile in position " + processed +
               "? Please specify letter as in 'Yx <letter>'.\n";
    } else if (command.find("exchange") != std::string::npos) {
        return "Are you trying to exchange one of your tiles? Please specify letter as in 'exchange <letter>'.\n";
    } else {
        return "Command not recognized. Please type 'help' to view the available commands.\n";
    }
}
    
bool isAlpha(std::string toTest) {
    for(auto &letter : toTest){
        if(!std::isalpha(letter))
            return false;
    }
    return true;
}