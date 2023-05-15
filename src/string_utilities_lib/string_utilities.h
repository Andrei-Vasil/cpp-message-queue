#ifndef STRING_UTILITIES
#define STRING_UTILITIES

#include <vector>
#include <string>

std::vector<std::string> split(std::string str, std::string delim) {
    std::vector<std::string> result = std::vector<std::string>();

    int end = str.find(delim); 
    while (end != -1) { // Loop until no delimiter is left in the string.
        result.push_back(str.substr(0, end));
        str.erase(str.begin(), str.begin() + end + 1);
        end = str.find(delim);
    }
    result.push_back(str.substr(0, end));
    return result;
}

#endif // STRING_UTILITIES
