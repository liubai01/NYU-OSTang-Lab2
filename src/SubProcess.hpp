#ifndef SUBPROCESS_H // include guard
#define SUBPROCESS_H

#include<vector>
#include<iostream>
#include<string>
#include <unistd.h>
#include "Job.hpp"

using namespace std;

#define WRITEF 1
#define APPENDF 2
#define READF 3
#define OTHERF 0

class SubProcess {
public:
    SubProcess();

    int inDp;
    int outDp;

    int inDpMode;
    int outDpMode;

    pid_t pid;
    pid_t pgid;

    string inPath;
    string outPath;


    bool first;
    bool active;

    Job* parentJob;

    vector<string> args;

    void exec(vector<int>& closeList);

    string getInfo();
    void debugPrint();

    bool setInDp(int dp, string filePath, int mode);
    bool setOutDp(int dp, string filePath, int mode);
};

#endif 