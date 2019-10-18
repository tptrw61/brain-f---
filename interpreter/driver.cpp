#include "bfe.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>

std::vector<std::string> split(std::string s, size_t length) {
	std::vector<std::string> splitString;
	splitString.push_back("");
	for (char c : s) {
		if (splitString.back().size() == length)
			splitString.push_back("");
		splitString.back().push_back(c);
	}
	return splitString;
}

void bfe_perror(Error error, std::string prog = "") {
	if (error.error != BFE_ERR_NONE) {
		std::cerr << "Error";
		switch (error.error) {
		case BFE_ERR_FUNC_CALL:
			std::cerr << " FUNC_CALL";
			break;
		case BFE_ERR_FUNC_DECL:
			std::cerr << " FUNC_DECL";
			break;
		case BFE_ERR_PAREN_CLOSE:
			std::cerr << " PAREN_CLOSE";
			break;
		case BFE_ERR_PAREN_OPEN:
			std::cerr << " PAREN_OPEN";
			break;
		default:
			break;
		}
		if (error.pos >= 0)
			std::cerr << " at " << error.pos;
		std::cerr << std::endl;
		std::vector<std::string> splitString = split(prog, 80);
		size_t pos = 0;
		for (std::string s : splitString) {
			std::cerr << s << std::endl;
			for (size_t i = 0; i < s.size(); i++) {
				std::cerr << (error.pos == pos ? '^' : ' ');
				pos++;
			}
			std::cerr << std::endl;
		}
		exit(BFE_FAILURE);
	} else
		return;
}

int main(int argc, char **argv) {
	std::string methods = funcReadLine('r') + funcPrintString('p');
	/*
	std::string mult(' ', '+');
	std::string space = "{s" + mult + "}";
	mult = std::string(' ', '-');
	std::string unspace = "{u" + mult + "}";
	std::string prog = "!r!p>[>]!s[.<]";
	// */
	std::string prog = "!r" + writeString("You wrote: '") + "<[<]!p <[<] <[<]!p >[>] <.[<]" + writeString("\nBackwards its: '") + "<[<]!p <[<] <[.<] >[>] >[>] <.";
	//std::stringstream ss;
	//ss << "Hello world!";
	std::stringstream oss;
	prog += methods;
	Error err = checkSyntax(prog);
	bfe_perror(err, prog);
	if (argc > 1) {
		if (std::string("-p").compare(argv[1]) == 0) {
			std::cout << stripProg(prog) << std::endl;
			return 0;
		}
	}
	int rv = interpret(prog, oss, std::cin);
	assert(rv == BFE_SUCCESS);

	std::cout << oss.str() << std::endl;

	return rv;
}
