#ifndef JOB_H // include guard
#define JOB_H

#include <string>
#include <iostream>
#include <list>

using namespace std;


class Job {
public:

    Job();

    string cmd;
    pid_t lastPid; // pid for last subprocess

    int aliveSubNum;
    int activeSubNum;

    void debugPrint();
};

#endif 