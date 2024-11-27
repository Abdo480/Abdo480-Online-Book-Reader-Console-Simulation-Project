#include "Helper.h"
#include <sstream>
#include <fstream>
#include <regex>
#include <iostream>
#include <iomanip>
#include <ctime>
#pragma warning(disable : 4996)

int Helper::ReadNumberInRange(int from, int to) {
	int number;
	std::cout << "\nEnter a number in range " << from << " - " << to << ": ";
	
	while (!(std::cin >> number) || number < from || number > to) {
		if (!std::cin)
			std::cout << "\nInvalid input, number must be a numeric value, try again.\n";
		else
			std::cout << "\ninput out of range, try again\n";

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nEnter a number in range " << from << " - " << to << ": ";
	}
	
	return number;
}

//Validates Input
int Helper::ReadNumber() {
	int number;
	std::cin >> number;
	while (!std::cin) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nERROR: Enter a numeric value: ";
		std::cin >> number;
	}

	return number;
}

std::string Helper::GetCurrentDataTime() {
	// Get the current time as time_t
	std::time_t now = std::time(nullptr);

	// Convert to tm structure for local time
	std::tm* localTime = std::localtime(&now);

	// Use a stringstream to format the date and time
	std::ostringstream oss;
	oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS

	// Return the formatted string
	return oss.str();
}

int Helper::ReadMenuOption(const std::vector<std::string>& menu) {
	if (menu.size() == 0)
		return 0;

	std::cout << "\nMenu:";
	for (int i = 0; i < menu.size(); ++i)
		std::cout << "\t\t" << i + 1 << ": " << menu[i] << "\n";
	
	return ReadNumberInRange(1, (int)menu.size());
}

int Helper::ToInt(std::string str) {
	std::istringstream iss(str);
	int value;
	iss >> value;
	return value;
}

std::string Helper::ToString(int value) {
	std::istringstream iss(value);
	return iss.str();
}

bool Helper::isValidEmail(std::string const& email) {
	const std::regex pattern(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
	return std::regex_match(email, pattern);
}

void Helper::ReadFromFile(std::string const& fileName, std::vector<std::string>& out) {
	std::ifstream fileHandler(fileName);
	if (fileHandler.is_open()) {
		std::string str;
		while (std::getline(fileHandler, str)) {
			if (str.length() > 0)
				out.push_back(str);
		}
	}
	fileHandler.close();
}

void Helper::WriteLinesToFile(std::vector<std::string> const& lines, std::string const& fileName, bool append) {
	auto status = std::ios::app;
	if (!append)
		status = std::ios::trunc | std::ios::out;

	std::fstream fileHandler(fileName, status);
	if (fileHandler.is_open()) {
		for (auto const& line : lines) {
			if (line.size() > 0)
				fileHandler << line << "\n";
		}
	}

	fileHandler.close();
}

std::vector<std::string> Helper::SplitString(std::string const& line, std::string const& delimiter) {
	size_t pos = 0;
	std::vector<std::string> substrs;
	size_t findStartPos = 0;
	while ((pos = line.find(delimiter, findStartPos)) != std::string::npos) {
		std::string substr = line.substr(findStartPos, pos - findStartPos);
		substrs.push_back(substr);

		findStartPos = pos + delimiter.length();
	}

	if (findStartPos < line.size())
		substrs.push_back(line.substr(findStartPos, line.size()));
	return substrs;
}