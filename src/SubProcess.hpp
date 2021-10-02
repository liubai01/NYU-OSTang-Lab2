#ifndef SUBPROCESS_H // include guard
#define SUBPROCESS_H

#include<vector>
#include<iostream>
#include<string>
#include <unistd.h>
#include "Job.hpp"

using namespace std;


class SubProcess {
public:
    SubProcess();

    int inDp;
    int outDp;
    pid_t pid;

    Job* parentJob;

    vector<string> args;

    void exec(vector<int>& closeList);

    string getInfo();
    void debugPrint();

    bool setInDp(int dp);
    bool setOutDp(int dp);
};

#endif 