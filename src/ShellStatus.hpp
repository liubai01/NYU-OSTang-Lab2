#ifndef SHELLSTATUSCMD_H // include guard
#define SHELLSTATUSCMD_H

#include<list>
#include "SubProcess.hpp"
#include "Job.hpp"
#include <unistd.h>


using namespace std;

class ShellStatus
{
public:
    ShellStatus();

    list<SubProcess> subs;
    list<Job> jobs;

    bool hasSuspendedJob;

    int activeProcessNum;

    void registerJob(Job& j);

    void registerSubProcess(SubProcess& s, Job& j);
    void deleteSubProcess(pid_t pid);

    void debugPrint();

    void printJobs();
};

#endif 