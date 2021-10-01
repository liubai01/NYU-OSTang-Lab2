#ifndef NYUSHELL_H // include guard
#define NYUSHELL_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "cmd"
#include <sys/wait.h>
#include "ShellStatus.hpp"

using namespace std;


class NyuShell {
public:
	// command name to handler
    unordered_map<string, CmdBase*> mlt;
    // command handlers
    vector<CmdBase*> cmds;
    // cpid list
    unordered_set<pid_t> cpids;
    // shell status
    ShellStatus status;


    NyuShell();
    ~NyuShell();

    void serve();
    // wait all children process and main cpids list
    void waitUntilClear();
};




#endif 