#pragma once
#include <string>
#include <functional>
#include <vector>

using namespace std;

string generateHash(const string& data) {
    hash<string> hasher;
    size_t hashValue = hasher(data);

    stringstream ss;
    ss << hashValue;
    
    string result = ss.str();
    if (result.length() > 16) {
        result = result.substr(0, 16);
    }
    
    while (result.length() < 16) {
        result = "0" + result;
    }
    return result;
}

bool fuzzyMatch(const string& input, const string& target) {
    size_t pos = target.find(input);
    return pos != string::npos;
}

int binarySearch(const vector<string>& data, const string& key) {
    size_t low = 0, high = data.size();
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (data[mid] == key) return mid;
        if (data[mid] < key) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return -1;
}
