#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

class FileManager {
public:
    static std::string readFile(const std::string& filename);
    static void writeFile(const std::string& filename, const std::string& content);
    static std::string hashContent(const std::string& content);
};

#endif
