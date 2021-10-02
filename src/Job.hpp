#ifndef JOB_H // include guard
#define JOB_H

#include <string>
#include <iostream>
#include <list>

using namespace std;


class Job {
public:
    string cmd;
    int subnum;

    list<Job>::iterator listElem;

    void debugPrint();
};

#endif 