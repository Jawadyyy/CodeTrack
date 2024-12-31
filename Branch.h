#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

struct Commit {
    string message;
    string hash;
    time_t timestamp;
};

class Branch {
public:
    string name;
    vector<Commit> commits;

    void makeCommit(const string& message, const string& hash);
    void showCommitLog() const;
    bool revertCommit(size_t commitIndex);
};

void Branch::makeCommit(const string& message, const string& hash) {
    time_t now = time(nullptr);
    commits.push_back({message, hash, now});
}

void Branch::showCommitLog() const {
    if (commits.empty()) {
        cout << "No commits yet.\n";
        return;
    }
    for (size_t i = 0; i < commits.size(); ++i) {
        cout << i + 1 << ". " << commits[i].message
                  << " (Hash: " << commits[i].hash << ")\n"
                  << "Timestamp: " << put_time(localtime(&commits[i].timestamp), "%Y-%m-%d %H:%M:%S") << "\n";
    }
}

bool Branch::revertCommit(size_t commitIndex) {
    if (commitIndex > 0 && commitIndex <= commits.size()) {
        commits.resize(commitIndex);
        return true;
    }
    return false;
}
