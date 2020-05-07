#ifndef FILE_TO_STRING
#define FILE_TOSTRING

#include <fstream>
#include <vector>

std::string readFileToString(std::string filename){
	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open()){
		throw std::runtime_error("Cannot open file");
	}
	std::ifstream::pos_type fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> bytes(fileSize);
	file.read(bytes.data(), fileSize);
	return std::string(bytes.data(), fileSize);
}

#endif