#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv) {
	if (argc == 1) {
		std::cerr << "no file specified" << std::endl;
		return -1;
	}
	std::fstream fs(argv[1], std::fstream::in);
	std::string prog;
	{
		char c;
		long long brCt = 0;
		std::string valid = "+-<>[],.";
		while (fs.get(c)) {
			if (valid.find(c) != std::string::npos) {
				prog.push_back(c);
				if (c == '[')
					brCt++;
				if (c == ']')
					brCt--;
				if (brCt < 0) {
					std::cerr << "missing open bracket" << std::endl;
					return -1;
				}
			}
		}
		if (brCt != 0) {
			std::cerr << "missing close bracket" << std::endl;
			return -1;
		}
	}
	if (prog.size()) {
		while (prog[0] == '[') { //remove initial loop
			size_t ct = 1, i = 0;
			while (ct != 0) {
				i++;
				if (prog[i] == '[')
					ct++;
				if (prog[i] == ']')
					ct--;
			}
			prog.erase(0, i+1);
		}
		size_t pos;
		//removing +- -+ <> ><
		while ((pos = prog.find("+-")) != std::string::npos) {
			prog.erase(pos, 2);
		}
		while ((pos = prog.find("-+")) != std::string::npos) {
			prog.erase(pos, 2);
		}
		while ((pos = prog.find("<>")) != std::string::npos) {
			prog.erase(pos, 2);
		}
		while ((pos = prog.find("><")) != std::string::npos) {
			prog.erase(pos, 2);
		}
		//remove loop directly after loop ']['
		while ((pos = prog.find("][")) != std::string::npos) {
			pos++;
			size_t ct = 1, i = 0;
			while (ct != 0) {
				i++;
				if (prog[pos+i] == '[')
					ct++;
				if (prog[pos+i] == ']')
					ct--;
			}
			prog.erase(pos, i+1);
		}
	}
	std::cout << prog << std::endl;
	return 0;
}
