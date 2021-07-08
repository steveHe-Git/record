#include <iostream>
#include <vector>
#include <string>
#include <string.h>

using namespace std;

static vector<string> split(const string& str, const string& delim) {
    vector<string> result;

    if (str.empty()) {
        return result;
    }

    char* s = new char[str.length() + 1];
    strcpy(s, str.c_str());

    char* d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());
    
    char* token = strtok(s, d); 
    while(token) {
        string tmp = token;
        result.push_back(tmp);
        token = strtok(NULL, d);
    }
    free(s);
    free(d);

    return result;
}