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

pid_t execute(vector<string>& arglist)
{
    pid_t c_pid = fork();

    if (c_pid == 0)
    {
        int error;

        // parse input/output redirection
        while (arglist.size() >= 3)
        {
            string lastSecond = arglist[arglist.size() - 2];

            if(lastSecond == "<")
            {
                redirInput(arglist[arglist.size() - 1]);
                arglist.pop_back();
                arglist.pop_back();
            } else if (lastSecond == ">") {
                redirOutput(arglist[arglist.size() - 1], false);
                arglist.pop_back();
                arglist.pop_back();
            } else if (lastSecond == ">>") {
               redirOutput(arglist[arglist.size() - 1], true);
                arglist.pop_back();
                arglist.pop_back();
            } else {
                break;
            }

        }

        const char* cmd = arglist[0].c_str();
        // Remark: Note that the last element is nullptr as execvp required
        char* args[arglist.size() + 1] = {nullptr};

        for (int i = 0; i < arglist.size(); ++i) {
            args[i] = const_cast<char*>(arglist[i].c_str());
        }

        // children process - execuate command
        error = execvp(cmd, args);

        if (error == -1) {
             cout << "failed\n" << endl;
        }
        exit(1);
    }

    return c_pid;
}

inline void redirInput(string inputFile)
{
    // refer to: http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
    const char* inputFileC = inputFile.c_str();
    int in = open(inputFileC, O_RDONLY);
    dup2(in, 0);
    close(in);
}

inline void redirOutput(string outputFile, bool bAppend)
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