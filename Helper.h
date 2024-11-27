#pragma once
#include <string>
#include <vector>

class Helper {
public:
	static int ToInt(std::string str);
	static std::string ToString(int value);
	static void ReadFromFile(std::string const& fileName, std::vector<std::string>& out);
	static void WriteLinesToFile(std::vector<std::string> const& lines, std::string const& fileName, bool append = true);
	static std::vector<std::string> SplitString(std::string const& line, std::string const& delimiter = ",");
	static bool  isValidEmail(std::string const& email);
	static int ReadMenuOption(const std::vector<std::string>& menu);
	static int ReadNumberInRange(int from, int to);
	static int ReadNumber();
	static std::string GetCurrentDataTime();
};