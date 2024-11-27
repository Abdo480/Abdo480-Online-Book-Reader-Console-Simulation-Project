#include "MyExceptions.h"

DanglingException::DanglingException(std::string const& msg, int val) : msg(msg), val(val) {}

const std::string& DanglingException::GetMessage() const { return msg; }

int DanglingException::what() const { return val; }