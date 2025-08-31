#include "VCS.h"
#include <filesystem>
namespace fs = std::filesystem;

// Generate unique commit ID
std::string generateCommitId() {
    time_t now = std::time(nullptr);
    return "commit_" + std::to_string(now);
}

// CommitNode constructor
CommitNode::CommitNode(std::string msg, CommitNode* parentNode)
    : message(msg), parent(parentNode), timestamp(std::time(nullptr)) {
    commitId = generateCommitId();
}

// VCS constructor
VCS::VCS() {
    HEAD = nullptr;
    currentBranch = "main";
}

// Initialize repository
void VCS::init() {
    std::cout << "Initializing MiniGit repository...\n";
    CommitNode* initial = new CommitNode("Initial commit", nullptr);
    HEAD = initial;
    branches[currentBranch] = initial;
    std::cout << "Repository initialized. HEAD -> " << HEAD->commitId << "\n";
}

// Add file to staging area
void VCS::addFile(std::string filename) {
    std::string content = FileManager::readFile(filename);
    if (content.empty()) {
        std::cout << "File not found or empty: " << filename << "\n";
        return;
    }

    std::string contentHash = FileManager::hashContent(content);
    stagingArea[filename] = contentHash;

    std::cout << "Added to staging: " << filename << " [hash: " << contentHash << "]\n";
}

// Commit staged files
void VCS::commit(const std::string& message) {
    if (stagingArea.empty()) {
        std::cout << "No changes to commit.\n";
        return;
    }

    CommitNode* newCommit = new CommitNode(message, HEAD);
    newCommit->fileHashes = stagingArea;
    HEAD->children.push_back(newCommit);
    HEAD = newCommit;
    branches[currentBranch] = HEAD;

    // Save files to repo
    std::string commitPath = "repo/" + newCommit->commitId;
    fs::create_directories(commitPath);

    for (auto& entry : stagingArea) {
        std::string filename = entry.first;
        std::string content = FileManager::readFile(filename);
        FileManager::writeFile(commitPath + "/" + filename, content);
    }

    stagingArea.clear();
    std::cout << "Committed successfully: " << newCommit->commitId << "\n";
}

// Show commit history
void VCS::log() {
    CommitNode* temp = HEAD;
    std::cout << "=== Commit History ===\n";
    while (temp) {
        std::cout << "Commit ID: " << temp->commitId << "\n";
        std::cout << "Message: " << temp->message << "\n";
        std::cout << "Timestamp: " << std::ctime(&temp->timestamp);
        std::cout << "-------------------------\n";
        temp = temp->parent;
    }
}

// Find commit (DFS)
CommitNode* findCommit(CommitNode* node, const std::string& id) {
    if (!node) return nullptr;
    if (node->commitId == id) return node;
    for (auto* child : node->children) {
        CommitNode* res = findCommit(child, id);
        if (res) return res;
    }
    return nullptr;
}

// Checkout commit
void VCS::checkoutCommit(std::string commitId) {
    CommitNode* target = findCommit(branches["main"], commitId);
    if (!target) {
        std::cout << "Commit not found: " << commitId << "\n";
        return;
    }

    std::string commitPath = "repo/" + commitId;
    for (auto& [filename, hash] : target->fileHashes) {
        std::string content = FileManager::readFile(commitPath + "/" + filename);
        FileManager::writeFile(filename, content);
    }

    HEAD = target;
    std::cout << "Checked out to commit: " << commitId << "\n";
}

// Branch creation
void VCS::branch(std::string name) {
    if (branches.find(name) != branches.end()) {
        std::cout << "Branch already exists: " << name << "\n";
        return;
    }
    branches[name] = HEAD;
    std::cout << "Branch '" << name << "' created at commit: " << HEAD->commitId << "\n";
}

// Switch branch
void VCS::switchBranch(std::string name) {
    if (branches.find(name) == branches.end()) {
        std::cout << "Branch not found: " << name << "\n";
        return;
    }
    if (name == currentBranch) {
        std::cout << "Already on branch: " << name << "\n";
        return;
    }
    currentBranch = name;
    HEAD = branches[name];

    std::string commitPath = "repo/" + HEAD->commitId;
    for (auto& [filename, hash] : HEAD->fileHashes) {
        std::string content = FileManager::readFile(commitPath + "/" + filename);
        FileManager::writeFile(filename, content);
    }

    std::cout << "Switched to branch: " << name << "\n";
}
