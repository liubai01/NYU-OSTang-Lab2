#ifndef SHELLSTATUSCMD_H // include guard
#define SHELLSTATUSCMD_H

#include<list>
#include "SubProcess.hpp"
#include <unistd.h>


using namespace std;

class ShellStatus
{
public:
    list<SubProcess> subs;
    bool hasSuspendedJob;

    void registerSubProcess(SubProcess& s);
    void deleteSubProcess(pid_t pid);

    void debugPrint();
};

#endif 