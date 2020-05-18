#include "../common/containerUtil.h"
#include "../common/consoleUtil.h"
#include <algorithm>

static std::mt19937 RANDOM_GENERATOR;

/**
 * Lowercase all chars.
 * @param command - string to be treated.
 */
void Util::lowerCase(std::string& str) {
	for (char& letter : str) letter = static_cast<char>(std::tolower(letter));
}

/**
 * Uppercase all chars.
 * @param command - string to be treated.
 */
void Util::upperCase(std::string &str) {
    for(char &letter : str) letter = static_cast<char>(std::toupper(letter));
}

/**
 * Remove unnecessary spaces of a string (at the end, the beginning, or more than once in between)
 * @param str - string to be treated.
 */
void Util::stripUnnecessarySpaces(std::string& str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (SPACE == str.at(i)) {
			if (str.length() - 1 == i) {
				str.erase(i, 1);
				break;
			}
			else if (0 == i || SPACE == str.at(i + 1)) {
				str.erase(i, 1);
				i--;
			}
		}
	}
}

/**
 * Remove all the spoaces from a string
 * @param str - String passed through reference to remove
 */
void Util::stripAllSpaces(std::string &str) {
    std::string output;
    for(const char &character : str){
        if (character != SPACE)
            output += character;
    }
    str = output;
}


/**
 * Uppercase word initials (a space marks a new word).
 * @param name - string to be treated.
 */
void Util::upperNameInitials(std::string& name) {
	bool doUpper = true;
	for (char & i : name) {
		char current = i;
		if (doUpper && std::isupper(current)) doUpper = false;
		else if (doUpper && std::islower(current)) {
			i = static_cast<char>(toupper(i));
			doUpper = false;
		}
		else if (SPACE == current) doUpper = true;
	}
}

/**
 * Remove special chars of a string.
 * @param name - string to be treated.
 * @param acceptDigits - if digits are considered regular chars.
 */
void Util::stripSpecialChars(std::string& name, bool acceptDigits) {
	std::string cleanStr;
	for (const char& c : name) {
		if (std::isalpha(c) || c == SPACE || (acceptDigits && std::isdigit(c))) {
			cleanStr.push_back(c);
		}
	}
	name = cleanStr;
}

/**
 * Output a string to the console with padding and max width (defined in the header file)
 * @param text - text to be output to the console
 * */
void Util::stringWriter(const std::string& text) {
	size_t lineCount = 0, len = text.size();
	char current;

	std::cout << LEFT_PADDING_STR;
	for (size_t i = 0; i < len; ++i) {
		current = text.at(i);
		if (lineCount == 0 && current == SPACE) continue;
		else if (current == '\n') {
			std::cout << std::endl;
			if (i != len - 1)
			    std::cout << LEFT_PADDING_STR;
			lineCount = 0;
		}
		else if (lineCount >= MAX_TEXT_WIDTH && i < len - 1 && text.at(i+1) == SPACE) {
			std::cout << current << std::endl << LEFT_PADDING_STR;
			lineCount = 0;
		}
		else {
			std::cout << current;
			lineCount++;
		}
	}
}

/**
 * Remove common command keywords from a string.
 * @param command - string to be treated.
 */
void Util::stripCommandBloat(std::string& command) {
	std::vector<std::string> keywords = //careful not to interfere with board coords or reserved commands
	{ "play","move","from","letter","check","get","position","board","load","save","write","show", "tile" };

	for (const std::string& word : keywords) {
		int pos = command.find(word);
		if (command.find(word) != std::string::npos) {
			command.erase(pos, pos + word.size());
		}
	}
}

/**
 * Get contextual advice based on the command.
 * @param command - string whose content is to be analysed.
 * @return string with advice.
 */
std::string Util::smartCommandAdvice(const std::string& command) {
	if (command.size() == 2 && std::isalpha(command.at(0)) && std::isalpha(command.at(1))) {
		std::stringstream processed;
		processed << static_cast<char>(std::toupper(command.at(0))) << static_cast<char>(std::tolower(command.at(1)));
		std::string processedStr = processed.str();
		return "Did you attempt to play a tile on position " + processedStr +
			"? Please specify letter: '" + processedStr + " <letter>'.\n";
	}
	else if (command.find("exchange") != std::string::npos) {
		return "Are you trying to exchange one of your tiles? Please specify letter: 'exchange <letter>'.\n";
	}
	else {
		return "Command not recognized. Please type 'help' to view the available commands.\n";
	}
}

/**
 * Check if chars of a strings are all alphabetical.
 * @param toTest - string to be tested.
 * @param acceptSpaces - wheather spaces are considered alpha chars.
 * @return true - string is all alpha chars.
 * @return false - string contains special chars, or is empty.
 */
bool Util::isAlpha(const std::string& toTest, bool acceptSpaces) {
	for (const auto& letter : toTest) {
		if (!std::isalpha(letter)) {
			if (letter == SPACE) {
				if (acceptSpaces) continue;
				else return false;
			}
			else return false;
		}
	}
	return !toTest.empty();
}

/**
 * Check if chars of a string are all digits ('0' to '9').
 * @param toTest - string to be tested.
 * @return true - string is all digits.
 * @return false - string contains non-digits, or is empty.
 */
bool Util::isDigit(const std::string& toTest) {
	for (const auto& letter : toTest) if (!std::isdigit(letter)) return false;
	return !toTest.empty();
}

/**
 * Check is the stream buffer has characters available to be output.
 * @param ss - stringstream to be treated.
 * @return true - no chars in buffer.
 * @return false - one or more chars in buffer.
 */
bool Util::isEmpty(const std::stringstream& ss) {
	return ss.rdbuf()->in_avail() == 0;
} 

/** Generate seed based on current local time and uses it to construct the random generator. */
void Util::initRandom() {
	unsigned int seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
	RANDOM_GENERATOR = std::mt19937(seed);
}

/**
 * Get random int between given bounds.
 * @param lowerBound - inclusive.
 * @param upperBound - inclusive.
 * @return random int between bounds.
 */
int Util::randomBetween(int lowerBound, int upperBound) {
	std::uniform_int_distribution<int> distribution{ lowerBound, upperBound };
	return distribution(RANDOM_GENERATOR);
}

/**
 * Shuffle the elements of a vector of chars.
 * @param v - container to be iterated.
 */
void Util::shuffle(std::vector<char> &v){
	std::shuffle(v.begin(), v.end(), RANDOM_GENERATOR);
}