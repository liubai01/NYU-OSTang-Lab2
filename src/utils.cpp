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
    if (args.size() > 0)
    {
        ret.emplace_back(args);
    }

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
         cout << "failed" << endl;
    }
    exit(1);

}

void parseRedirFile(vector<string>& args)
{
    while (args.size() >= 3)
    {
        string lastSecond = args[args.size() - 2];

        if(lastSecond == "<")
        {
            redirInputFile(args[args.size() - 1]);
            args.pop_back();
            args.pop_back();
        } else if (lastSecond == ">") {
            redirOutputFile(args[args.size() - 1], false);
            args.pop_back();
            args.pop_back();
        } else if (lastSecond == ">>") {
           redirOutputFile(args[args.size() - 1], true);
            args.pop_back();
            args.pop_back();
        } else {
            break;
        }

    }
}

void redirInputFile(string inputFile)
{
    // refer to: http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
    const char* inputFileC = inputFile.c_str();
    int in = open(inputFileC, O_RDONLY);
    dup2(in, 0);
    close(in);
}

void redirOutputFile(string outputFile, bool bAppend)
{
    // refer to: http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
    const char* outputFileC = outputFile.c_str();
    int out;
    if (!bAppend)
    {
        out = open(outputFileC, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    } else {
        out = open(outputFileC, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    }
    
    dup2(out, 1);
    close(out);
}