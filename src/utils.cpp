#include "utils.hpp"
#include <unistd.h>

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

pid_t execute(vector<string>& arglist)
{
    int error;

    const char* cmd = arglist[0].c_str();
    char* args[arglist.size() + 1] = {nullptr};

    for (int i = 0; i < arglist.size(); ++i) {
        args[i] = const_cast<char*>(arglist[i].c_str());
    }

    pid_t c_pid = fork();

    if (c_pid == 0)
    {
        // children process - execuate command
        error = execvp(cmd, args);

        if (error == -1) {
             cout << "failed\n" << endl;
        }
        exit(1);
    }

    return c_pid;
}