#include "CodeTrack.h"

int main()
{
    CodeTrack tracker;
    string command;

    tracker.displayLogo();

    while (true) {
        cout << "\nEnter a command (type 'help' for options): ";
        getline(cin, command);

        if (command == "help") {
            tracker.help();
        } else if (command == "ct init") {
            tracker.ctInit();
        } else if (command.rfind("ct add", 0) == 0) {
            string file = command.substr(7);
            tracker.ctAdd(file);
        } else if (command.rfind("ct commit -m", 0) == 0) {
            string message = command.substr(13);
            tracker.ctCommit(message);
        } else if (command.rfind("ct revert", 0) == 0) {
            string commitHash = command.substr(10);

            commitHash.erase(0, commitHash.find_first_not_of(" \t\n\r"));
            commitHash.erase(commitHash.find_last_not_of(" \t\n\r") + 1);

            if (commitHash.empty()) {
                cout << "\033[31mError: Please provide a commit hash after 'ct revert'.\033[0m\n";
            }
            else {
                tracker.ctRevert(commitHash);
            }
        } else if (command == "ct log") {
            tracker.ctLog();
        } else if (command == "ct status") {
            tracker.ctStatus();
        } else if (command.rfind("ct binarySearch", 0) == 0) {
            string key = command.substr(15);

            key.erase(0, key.find_first_not_of(" \t\n\r"));
            key.erase(key.find_last_not_of(" \t\n\r") + 1, key.length());

            if (key.length() != 16) {
                cout << "\033[31mError: Commit hash must be exactly 16 characters long.\033[0m\n";
                return -1;
            }

            tracker.ctBinarySearch(key);
        } else if (command == "exit") {
            cout << "\033[32mExiting the program.\033[0m\n";
            break;
        } else {
            cout << "\033[31mInvalid command.\033[0m\n";
            tracker.ctFuzzyMatch(command);
        }
    }

    return 0;
}
