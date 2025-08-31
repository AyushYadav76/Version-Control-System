#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <functional>

// Read file into string
std::string FileManager::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return "";

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Write string to file
void FileManager::writeFile(const std::string& filename, const std::string& content) {
    std::ofstream out(filename);
    out << content;
}

// Hash string content
std::string FileManager::hashContent(const std::string& content) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(content));
}
