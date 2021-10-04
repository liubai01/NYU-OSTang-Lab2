#include "utils.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

string getMyCwd() 
{
    char tmp[256];

    getcwd(tmp, 256);
    string ret(tmp);

    return ret;
}

int changedir(string path)
{
    return chdir(path.c_str());
}

vector<string> splitStr(string s, string delimiter) {
    vector<string> ret;
    
    auto start = 0U;
    auto end = s.find(delimiter);
    while (end != std::string::npos)
    {
        ret.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    if (start < s.length())
    {
        ret.push_back(s.substr(start, s.length() - start + 1));
    }

    return ret;
}

vector<vector<string>> splitTokens(vector<string>& tokens, string delimiter)
{
    vector<vector<string>> ret;
    vector<string> args;

    for (auto& a: tokens)
    {
        if (a == delimiter)
        {
            ret.emplace_back(args);
            args.clear();
        } else {
            args.push_back(a);
        }
    }

    ret.emplace_back(args);

    return ret;
}

void execute(vector<string>& arglist)
{
    int error;

    const char* cmd = arglist[0].c_str();
    // Remark: Note that the last element is nullptr as execvp required
    char* args[arglist.size() + 1] = {nullptr};

    for (int i = 0; i < arglist.size(); ++i) {
        args[i] = const_cast<char*>(arglist[i].c_str());
    }

    // children process - execuate command
    error = execvp(cmd, args);

    if (error == -1) {
         cerr << "Error: invalid command" << endl;
    }
    exit(1);

}