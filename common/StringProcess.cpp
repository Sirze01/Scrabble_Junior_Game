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

std::string stripCommandBloat(std::string command) {
    //an experiment on smart command interpretation. might remove?
    std::string cleanCommand(command);
    std::vector<std::string> keywords = //careful not to interfere with board coords or other commands
    { "play","move","from","letter","check","get","position","board","load","save","write","show"};

    for (std::string word : keywords) {
        int pos = cleanCommand.find(word);
        if (cleanCommand.find(word) != std::string::npos) {
            cleanCommand.erase(pos, pos + word.size());
        }
    }
    return cleanCommand;
}