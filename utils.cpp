#include "utils.hpp"

string getMyCwd() {
    char tmp[256];
    getcwd(tmp, 256);
    string ret(tmp);
    return ret;
}

vector<string> splitCwd(string s, string delimiter) {
    vector<string> ret;

	size_t pos = 0;
	string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        ret.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    return ret;
}