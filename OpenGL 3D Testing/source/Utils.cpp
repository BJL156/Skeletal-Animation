#include "Utils.h"

std::string readFileContents(const char *filepath) {
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cout << "Failed to read file: " << filepath << '\n';
		return nullptr;
	}

	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	return contents;
}