#include "VCS.h"

int main() {
    VCS vcs;
    std::string command;

    std::cout << "MiniGit CLI\n";
    while (true) {
        std::cout << "MiniGit> ";
        std::cin >> command;

        if (command == "init") {
            vcs.init();
        } else if (command == "add") {
            std::string filename;
            std::cin >> filename;
            vcs.addFile(filename);
        } else if (command == "commit") {
            std::string msg;
            std::getline(std::cin >> std::ws, msg);
            vcs.commit(msg);
        } else if (command == "log") {
            vcs.log();
        } else if (command == "checkout") {
            std::string id;
            std::cin >> id;
            vcs.checkoutCommit(id);
        } else if (command == "branch") {
            std::string name;
            std::cin >> name;
            vcs.branch(name);
        } else if (command == "switch") {
            std::string name;
            std::cin >> name;
            vcs.switchBranch(name);
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Unknown command.\n";
        }
    }
    return 0;
}
