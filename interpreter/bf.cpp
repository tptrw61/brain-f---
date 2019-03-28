#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//#define DEBUG

using cell = unsigned char;

int main(int argc, char **argv) {
#ifndef DEBUG
	if (argc == 1) {
		std::cerr << "no file specified" << std::endl;
		return -1;
	}
	std::fstream fs(argv[1], std::fstream::in);
#else
	std::string fname;
	std::cin >> fname;
	std::cin.ignore(1);
	std::fstream fs(fname, std::fstream::in);
#endif
	std::string prog;
	{
		char c;
		long brCt = 0;
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

#ifdef DEBUG
	std::clog << "here 1" << std::endl;
#endif

	unsigned long i, p;
	i = p = 0;
	std::vector<cell> data(100, 0);

	while (i < prog.size()) {
		if (prog[i] == '+')
			data[p]++;
		else if (prog[i] == '-')
			data[p]--;
		else if (prog[i] == '<') {
			if (p == 0)
				data.insert(data.begin(),0);
			else
				p--;
		}
		else if (prog[i] == '>') {
			p++;
			if (p == data.size())
				data.push_back(0);
		}
		else if (prog[i] == '[' && data[p] == 0) {
			unsigned brCt = 1;
			while (brCt != 0) {
				i++;
				if (prog[i] == '[')
					brCt++;
				if (prog[i] == ']')
					brCt--;
			}
		}
		else if (prog[i] == ']' && data[p] != 0) {
			unsigned brCt = 1;
			while (brCt != 0) {
				i--;
				if (prog[i] == ']')
					brCt++;
				if (prog[i] == '[')
					brCt--;
			}
		}
		else if (prog[i] == ',')
			data[p] = std::cin.eof() ? 0 : std::cin.get();
		else if (prog[i] =='.')
			std::cout << (char)data[p];
		i++;
	}
	return 0;
}
