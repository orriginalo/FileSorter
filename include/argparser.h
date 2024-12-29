#pragma once
#include <string>
std::string parseArgValue(int argc, char *argv[], std::string option, std::string shortOption);
bool checkArg(int argc, char *argv[], std::string option, std::string shortOption);