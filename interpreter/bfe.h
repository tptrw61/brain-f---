#pragma once
#include <string>
#include <iostream>

using Cell = int;
constexpr Cell DEFAULT_CELL = 0;

constexpr int BFE_SUCCESS = 0;
constexpr int BFE_FAILURE = 1;
constexpr int BFE_SYNTAX =  2;

constexpr int BFE_ERR_NONE =         0;
constexpr int BFE_ERR_PAREN_OPEN =   1;
constexpr int BFE_ERR_PAREN_CLOSE =  2;
constexpr int BFE_ERR_FUNC_DECL =   10;
constexpr int BFE_ERR_FUNC_CALL =   11;

struct Error {
	Error() : pos(-1), error(BFE_ERR_NONE) {}
	Error(int _pos, int _error) :pos(_pos), error(_error) {}
	size_t pos;
	int error;
};

//runs the program through the interpreter
int interpret(const std::string& prog, std::ostream& os = std::cout, std::istream& is = std::cin);

//checks the program for syntax errors and returns the position it occurred or -1 if no error occurred
Error checkSyntax(const std::string& prog);

//returns a copy of the program with all functions inlined
std::string inlineFunctions(const std::string& prog);

//returns true if successfully cross-compiles
bool compile(const std::string& prog, const std::string& outputFile);

std::string stripProg(std::string prog);

std::string funcReadLine(char funcName);
std::string funcPrintString(char funcName);
std::string writeString(std::string str, bool useLoop = true);
std::string writeNum(int num, bool useLoop = true);
