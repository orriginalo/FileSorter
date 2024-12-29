#include "../include/argparser.h"

using namespace std;

std::string parseArgValue(int argc, char *argv[], std::string option, std::string shortOption) {
  for (int i = 0; i < argc - 1; i++) { // Убедиться, что i + 1 не выходит за пределы
    if (argv[i] == option || argv[i] == shortOption) {
      return argv[i + 1];
    }
  }
  return ""; // Возврат пустой строки, если аргумент не найден
}

bool checkArg(int argc, char *argv[], std::string option, std::string shortOption) {
  for (int i = 0; i < argc; i++) {
    if (argv[i] == option || argv[i] == shortOption) {
      return true;
    }
  }
  return false;
}