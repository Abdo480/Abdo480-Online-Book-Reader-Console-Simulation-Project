#pragma once
#include <string>

class DanglingException {
private:
	std::string msg;
	int val;

public:
	DanglingException(std::string const& msg, int val);
	const std::string& GetMessage() const;
	int what() const;
};