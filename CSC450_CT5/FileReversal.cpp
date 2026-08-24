/*
 * CSC450 Critical Thinking 5
 * User Input and File Reversal Program
 *
 * Appends validated user input to CSC450_CT5_mod5.txt.
 * It then reverses every byte in the updated file and
 * writes the result to CSC450-mod5-reverse.txt.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

namespace {
	const std::string INPUT_FILE = "CSC450_CT5_mod5.txt";
	const std::string REVERSE_FILE = "CSC450-mod5-reverse.txt";
	const std::size_t MAXIMUM_INPUT_LENGTH = 1000;

	// Checks for control characters to avoid malformed text
	bool containsUnsupportedCharacter(const std::string& text) {
		for (unsigned char character : text) {
			if ((character < 32 && character != '\t') || character == 127) {
				return true;
			}
		}

		return false;
	}

	// Validates one line of user input
	bool getUserInput(std::string& userInput) {
		while (true) {
			std::cout << "Enter one line of text to append: ";

			if (!std::getline(std::cin, userInput)) {
				return false;
			}

			if (userInput.empty()) {
				std::cout << "Input cannot be empty. Please try again.\n";
			} else if (userInput.length() > MAXIMUM_INPUT_LENGTH) {
				std::cout
					<< "Input cannot exceed 1000 characters. Please try again.\n";
			} else if (containsUnsupportedCharacter(userInput)) {
				std::cout
					<< "Input contains an unsupported control character. "
					<< "Please try again.\n";
			} else {
				return true;
			}
		}
	}

	// Appends user's input without deleting existing file contents
	bool appendToFile(const std::string& userInput) {
		std::ifstream inputFile(INPUT_FILE, std::ios::binary);

		if (!inputFile) {
			std::cerr << "Error: Unable to open " << INPUT_FILE << ".\n";
			return false;
		}

		bool needsNewline = false;

		inputFile.seekg(0, std::ios::end);

		if (inputFile.tellg() > std::streampos(0)) {
			inputFile.seekg(-1, std::ios::end);

			char lastCharacter;
			inputFile.get(lastCharacter);
			needsNewline = lastCharacter != '\n';
		}

		inputFile.close();

		std::ofstream outputFile(
			INPUT_FILE,
			std::ios::binary | std::ios::app);

		if (!outputFile) {
			std::cerr << "Error: Unable to append to " << INPUT_FILE << ".\n";
			return false;
		}

		if (needsNewline) {
			outputFile << '\n';
		}

		outputFile << userInput << '\n';

		if (!outputFile) {
			std::cerr << "Error: Writing to " << INPUT_FILE << " failed.\n";
			return false;
		}
		return true;
	}

	// Reverses all characters in the source file and creates output file
	bool createReversedFile() {
		std::ifstream inputFile(INPUT_FILE, std::ios::binary);

		if (!inputFile) {
			std::cerr << "Error: Unable to read " << INPUT_FILE << ".\n";
			return false;
		}

		std::string fileContents{
			std::istreambuf_iterator<char>{inputFile},
			std::istreambuf_iterator<char>{}
		};

		if (inputFile.bad()) {
			std::cerr << "Error: Reading " << INPUT_FILE << " failed.\n";
			return false;
		}

		std::reverse(fileContents.begin(), fileContents.end());

		std::ofstream outputFile(
			REVERSE_FILE,
			std::ios::binary | std::ios::trunc);

		if (!outputFile) {
			std::cerr << "Error: Unable to create " << REVERSE_FILE << ".\n";
			return false;
		}

		outputFile.write(
			fileContents.data(),
			static_cast<std::streamsize>(fileContents.size()));

		if (!outputFile) {
			std::cerr << "Error: Writing to " << REVERSE_FILE << " failed.\n";
			return false;
		}
		return true;
	}
}

int main() {
    std::cout
        << "CSC450 File Append and Reversal Program\n"
        << "Source file: " << INPUT_FILE << '\n'
        << "Reverse file: " << REVERSE_FILE << "\n\n";

    std::string userInput;

    if (!getUserInput(userInput)) {
        std::cerr << "\nError: Input ended before text was entered.\n";
        return 1;
    }

    if (!appendToFile(userInput)) {
        return 1;
    }

    if (!createReversedFile()) {
        return 1;
    }

    std::cout
        << "\nThe input was appended without deleting the existing data.\n"
        << "The reversed contents were written to "
        << REVERSE_FILE << ".\n";

    return 0;
}
