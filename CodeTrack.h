#ifndef CODETRACK_H
#define CODETRACK_H

#include "Branch.h"
#include "Doubly.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;
namespace fs = filesystem;

DoublyLinkedList<Branch> branches;
Branch* currentBranch = nullptr;
unordered_map<string, string> stagingArea;

vector<string> commands = {"ct init", "ct add", "ct commit", "ct revert", "ct log", "ct status", "ct binarySearch"};

class CodeTrack {
public:
    void displayLogo();
    void help();
    void ctInit();
    void ctAdd(const string& file);
    void ctCommit(const string& message);
    void ctRevert(const string& commitHash);
    void ctLog();
    void ctStatus();
    void ctBinarySearch(const string& key);
    void ctFuzzyMatch(const string& input);
};

void CodeTrack::displayLogo() {
    cout << " CCCCC    OOO00    DDDD    EEEEE      TTTTTTTT   RRRR     AAAAA   CCCCC   K   K\n";
    cout << " C        O   O    D   D   E             TT      R  R     A   A   C       K  K \n";
    cout << " C        O   O    D   D   EEEE          TT      RRRR     AAAAA   C       KKk  \n";
    cout << " C        O   O    D   D   E             TT      R  R     A   A   C       K  K \n";
    cout << " CCCCC    OOO00    DDDD    EEEEE         TT      R   R    A   A   CCCCC   K   K\n";
}

void CodeTrack::help() {
    cout << "\nAvailable Commands:\n";
    cout << "1. ct init: Initialize the repository.\n";
    cout << "2. ct add <file_name|.>: Stage files for commit.\n";
    cout << "3. ct commit -m <message>: Commit staged changes with a message.\n";
    cout << "4. ct revert <commitID>: Revert to a specific commit.\n";
    cout << "5. ct log: Display commit history.\n";
    cout << "6. ct status: Show changes staged for commit.\n";
    cout << "7. ct binarySearch <key>: Perform a binary search for a commit hash.\n";
    cout << "8. exit: Exit the program.\n";
}

void CodeTrack::ctInit() {
    if (fs::exists(".codetrack")) {
        cout << "\033[33mRepository already initialized.\033[0m\n";
        return;
    }

    if (!currentBranch) {
        Branch* master = new Branch();
        master->name = "master";
        branches.insertAtEnd(master);
        currentBranch = master;
        fs::create_directory(".codetrack");
        cout << "\033[32mInitialized empty repository in ./.codetrack/\033[0m\n";
    } else {
        cout << "\033[33mRepository already initialized.\033[0m\n";
    }
}


void CodeTrack::ctAdd(const string& file) {
    if (!fs::exists(file)) {
        cout << "\033[31mError: File " << file << " does not exist.\033[0m\n";
        return;
    }

    if (fs::is_directory(file)) {
        for (const auto& entry : fs::recursive_directory_iterator(file)) {
            if (fs::is_regular_file(entry)) {
                ifstream inFile(entry.path());
                ostringstream content;
                content << inFile.rdbuf();
                stagingArea[entry.path().string()] = content.str();
            }
        }
    } else {
        ifstream inFile(file);
        ostringstream content;
        content << inFile.rdbuf();
        stagingArea[file] = content.str();
    }
    cout << "\033[32mFiles added to staging area.\033[0m\n";
}

void CodeTrack::ctCommit(const string& message) {
    if (stagingArea.empty()) {
        cout << "\033[33mNo changes to commit.\033[0m\n";
        return;
    }

    string combined;
    for (const auto& entry : stagingArea) {
        combined += entry.first + entry.second;
    }

    string hash = generateHash(combined);
    std::time_t now = std::time(nullptr);
    currentBranch->makeCommit(message, hash);

    ofstream outFile(".codetrack/" + hash);
    outFile << "Commit Hash: " << hash << "\n";
    outFile << "Message: " << message << "\n";
    outFile << "Timestamp: " << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << "\n";
    outFile << "--------------------------\n";
    outFile << combined;

    ofstream historyFile(".codetrack/history.txt", ios::app);
    historyFile << "Commit Hash: " << hash << "\n";
    historyFile << "Message: " << message << "\n";
    historyFile << "Timestamp: " << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << "\n";
    historyFile << "--------------------------\n";

    stagingArea.clear();
    cout << "\033[32mCommit successful with hash: " << hash << "\033[0m\n";
}

void CodeTrack::ctRevert(const string& commitHash) {
    bool reverted = false;

    if (currentBranch->commits.empty()) {
        cout << "\033[31mNo commits found in the current branch.\033[0m\n";
        return;
    }

    for (size_t i = 0; i < currentBranch->commits.size(); ++i) {
        if (currentBranch->commits[i].hash == commitHash) {
            currentBranch->commits.resize(i + 1);
            cout << "\033[32mReverted to commit: " << commitHash << ".\033[0m\n";
            reverted = true;
            break;
        }
    }

    if (!reverted) {
        cout << "\033[31mCommit hash not found.\033[0m\n";
    }
}


void CodeTrack::ctLog() {
    ifstream historyFile(".codetrack/history.txt");
    if (!historyFile) {
        cout << "\033[33mNo commit history available.\033[0m\n";
        return;
    }

    cout << "\033[36mCommit history:\033[0m\n";
    cout << "--------------------------\n";
    cout << historyFile.rdbuf();
}

void CodeTrack::ctStatus() {
    if (stagingArea.empty()) {
        cout << "\033[33mNo files staged for commit.\033[0m\n";
    } else {
        cout << "\033[36mFiles staged for commit:\033[0m\n";
        for (const auto& entry : stagingArea) {
            cout << "  " << entry.first << "\n";
        }
    }
}

void CodeTrack::ctBinarySearch(const string& key) {
    vector<string> commitHashes;
    
    for (const auto& entry : fs::directory_iterator(".codetrack")) {
        if (fs::is_regular_file(entry) && entry.path().filename().string().length() == 16) {
            commitHashes.push_back(entry.path().filename().string());
        }
    }

    sort(commitHashes.begin(), commitHashes.end());
    int index = binarySearch(commitHashes, key);
    
    if (index != -1) {
        string commitHash = commitHashes[index];
        cout << "\033[32mCommit hash found: " << commitHash << "\033[0m\n";
        
        ifstream commitFile(".codetrack/" + commitHash);
        if (commitFile) {
            string line;
            cout << "\033[36mCommit Details:\033[0m\n";
            
            while (getline(commitFile, line)) {
                if (line.find("Message:") == 0) {
                    cout << "\033[32mMessage: \033[0m" << line.substr(9) << "\n";
                }
                else if (line.find("Timestamp:") == 0) {
                    cout << "\033[32mTimestamp: \033[0m" << line.substr(11) << "\n";
                }
                else if (line.find("Files:") == 0) {
                    cout << "\033[32mCommitted Files:\033[0m\n";
                    while (getline(commitFile, line) && !line.empty()) {
                        cout << "  " << line << "\n";
                    }
                }
            }
        } else {
            cout << "\033[31mError: Commit file not found.\033[0m\n";
        }
    } else {
        cout << "\033[31mCommit hash not found.\033[0m\n";
    }
}

void CodeTrack::ctFuzzyMatch(const string& input) {
    string closestMatch;
    size_t maxMatchLength = 0;

    for (const string& command : commands) {
        if (command.find(input) != string::npos) {
            size_t matchLength = input.length();
            if (matchLength > maxMatchLength) {
                closestMatch = command;
                maxMatchLength = matchLength;
            }
        }
    }

    if (!closestMatch.empty()) {
        cout << "\033[32mDid you mean: " << closestMatch << "?\033[0m\n";
    } else {
        cout << "\033[31mNo command matches your input.\033[0m\n";
    }
}

#endif
