#ifndef VCS_H
#define VCS_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <string>
#include "FileManager.h"

struct CommitNode {
    std::string commitId;
    std::string message;
    std::time_t timestamp;
    std::unordered_map<std::string, std::string> fileHashes; // filename -> hash
    CommitNode* parent;
    std::vector<CommitNode*> children;

    CommitNode(std::string msg, CommitNode* parentNode);
};

class VCS {
private:
    CommitNode* HEAD;
    std::string currentBranch;
    std::unordered_map<std::string, CommitNode*> branches;
    std::unordered_map<std::string, std::string> stagingArea;

public:
    VCS();
    void init();
    void addFile(std::string filename);
    void commit(const std::string& message);
    void log();
    void checkoutCommit(std::string commitId);
    void branch(std::string name);
    void switchBranch(std::string name);
};

#endif
