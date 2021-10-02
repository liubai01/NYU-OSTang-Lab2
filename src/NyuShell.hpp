#ifndef NYUSHELL_H // include guard
#define NYUSHELL_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "cmd"
#include <sys/wait.h>
#include "ShellStatus.hpp"
#include "Job.hpp"

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

    int activeProcessNum;

    NyuShell();
    ~NyuShell();

    // wait all children process and main cpids list
    void waitUntilClear();
    // output prompt and parse the input
    vector<string> prompt();

    bool constrcutJobs(vector<vector<string>>& cmds, vector<int>& cleanUpList, vector<Job>& jobs);

    // start the shell service
    void serve();
};




#endif 