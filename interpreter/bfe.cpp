#include "bfe.h"
#include <stack>
#include <queue>
#include <vector>
#include <ctype.h>

constexpr char LEFT =    '<';
constexpr char RIGHT =   '>';
constexpr char INC =     '+';
constexpr char DEC =     '-';
constexpr char INPUT =   ',';
constexpr char OUTPUT =  '.';
constexpr char LBEGIN =  '[';
constexpr char LEND =    ']';

constexpr char FCALL =   '!';
constexpr char FDECL =   '{';
constexpr char FRET =    '}';


enum State {
	NoState,
	FuncCall,
	FuncDecl
};

static int left(int headPos, std::vector<int>& tape);
static int right(int headPos, std::vector<int>& tape);

int interpret(const std::string& prog, std::ostream& os, std::istream& is) {
	std::stack<int> loops;
	std::stack<int> instr;
	std::vector<Cell> tape;
	tape.push_back(DEFAULT_CELL);
	int headPos = 0;

	State state = NoState;

	int iptr = 0;
	char c;
	while (iptr < (int)prog.size()) {
		c = prog[iptr];
		if (state == FuncCall) {
			std::string s;
			s.push_back(FDECL);
			s.push_back(c);
			size_t pos = prog.find(s);
			if (pos == std::string::npos)
				return BFE_ERR_FUNC_CALL;
			instr.push(iptr);
			state = NoState;
			iptr = pos + 2;
			continue;
		} else
		if (state == FuncDecl) {
			if (c == FRET)
				state = NoState;
		} else
		if (c == FDECL) {
			state = FuncDecl;
		} else
		if (c == FRET) {
			iptr = instr.top();
			instr.pop();
		} else
		if (c == FCALL) {
			state = FuncCall;
		} else

		if (c == LEFT) {
			headPos = left(headPos, tape);
		} else
		if (c == RIGHT) {
			headPos = right(headPos, tape);
		} else
		if (c == INC) {
			tape[headPos]++;
		} else
		if (c == DEC) {
			tape[headPos]--;
		} else
		if (c == INPUT) {
			int input = is.get();
			if (input == '\n')
				input = 10;
			if (input == EOF)
				input = 0;
			tape[headPos] = input;
		} else
		if (c == OUTPUT) {
			if (isprint(c)) {
				os << (char)tape[headPos];
			} else {
				os << (int)tape[headPos];
			}
		} else
		if (c == LBEGIN) {
			if (tape[headPos] == 0) {
				int loopCount = 1;
				while (loopCount != 0) {
					iptr++;
					if (prog[iptr] == '[')
						loopCount++;
					if (prog[iptr] == ']')
						loopCount--;
				}
			} else {
				loops.push(iptr);
			}
		} else
		if (c == LEND) {
			iptr = loops.top();
			loops.pop();
			continue;
		}
		iptr++;
	}

	return BFE_SUCCESS;
}

int left(int headPos, std::vector<int>& tape) {
	if (headPos == 0) {
		tape.insert(tape.begin(), DEFAULT_CELL);
		return 0;
	}
	return headPos - 1;
}
int right(int headPos, std::vector<int>& tape) {
	if (headPos == (int)tape.size() - 1) {
		tape.push_back(DEFAULT_CELL);
	}
	return headPos + 1;
}

Error checkSyntax(const std::string& prog) {
	std::stack<size_t> loopStart;
	std::queue<size_t> funcs;
	bool inFunc = false;

	char c;
	for (size_t i = 0; i < prog.size(); i++) {
		c = prog[i];

		if (inFunc) {
			if (c == '}')
				inFunc = false;
		} else
		if (c == '{') {
			funcs.push(i);
			inFunc = true;
		} else
		if (c == '[') {
			loopStart.push(i);
		} else
		if (c == ']') {
			if (loopStart.empty())
				return Error(i, BFE_ERR_PAREN_CLOSE);
			loopStart.pop();
		} else
		if (c == '}') {
			return Error(i, BFE_ERR_PAREN_CLOSE);
		}
	}
	if (inFunc)
		return Error(funcs.front(), BFE_ERR_PAREN_OPEN);
	if (!loopStart.empty())
		return Error(loopStart.top(), BFE_ERR_PAREN_OPEN);

	while (!funcs.empty()) {
		for (size_t i = funcs.front() + 1; prog[i] != '}'; i++) {
			c = prog[i];

			if (c == '{') {
				return Error(i, BFE_ERR_PAREN_OPEN);
			} else
			if (c == '[') {
				loopStart.push(i);
			} else
			if (c == ']') {
				if (loopStart.empty())
					return Error(i, BFE_ERR_PAREN_CLOSE);
				loopStart.pop();
			}
		}
		if (!loopStart.empty())
			return Error(loopStart.top(), BFE_ERR_PAREN_OPEN);
		funcs.pop();
	}

	return Error();
}

std::string inlineFunctions(const std::string& prog) {
	return "";
}

bool compile(const std::string& prog, const std::string& outputFile) {
	return false;
}


/*
constexpr char LEFT =    '<';
constexpr char RIGHT =   '>';
constexpr char INC =     '+';
constexpr char DEC =     '-';
constexpr char INPUT =   ',';
constexpr char OUTPUT =  '.';
constexpr char LBEGIN =  '[';
constexpr char LEND =    ']';
constexpr char FCALL =   '!';
constexpr char FDECL =   '{';
constexpr char FRET =    '}';
*/
std::string stripProg(std::string prog) {
	std::string s;
	bool keepLetter = false;
	for (char c : prog) {
		if (keepLetter) {
			keepLetter = false;
			s.push_back(c);
			continue;
		}
		switch (c) {
		case FCALL:
		case FDECL:
			keepLetter = true;
		case LEFT:
		case RIGHT:
		case INC:
		case DEC:
		case INPUT:
		case OUTPUT:
		case LBEGIN:
		case LEND:
		case FRET:
			s.push_back(c);
		default:
			break;
		}
	}
	return s;
}

std::string funcReadLine(char funcName) {
	if (!isprint(funcName))
		return "";
	std::string s = "{";
	s.push_back(funcName);
	s += "[-]>[-]>[-]+[->[-]<<,[----------[++++++++++>>+<]]>]<}";
	return s;
}
std::string funcPrintString(char funcName) {
	if (!isprint(funcName))
		return "";
	std::string s = "{";
	s.push_back(funcName);
	s += ">[.>]}";
	return s;
}

std::string writeString(std::string str, bool useLoop) {
	std::string s = "[-]>";
	for (char c : str)
		s += writeNum(c, useLoop);
	if (str.empty())
		s += "[-]";
	return s;
}
std::string writeNum(int num, bool useLoop) {
	if (useLoop) {
		std::string s = "[-]>[-]<";
		if (num <= 10)
			return ">[-]<[-]" + std::string(num, '+') + ">";
		return "[-]" + std::string(num % 10, '+') + ">[-]" + std::string(num / 10, '+') + "[-<++++++++++>]";
	}
	return "[-]" + std::string(num, '+') + ">[-]";
}
