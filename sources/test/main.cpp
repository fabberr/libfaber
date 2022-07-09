#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <io.h>

int main() {

	faber::io::read_file("./sources/test/main.cpp", [](std::ifstream& file) -> bool {
		for (std::string line{}; std::getline(file, line); ) {
			std::cout << line << "\n";
		}
		return true;
	});
}
