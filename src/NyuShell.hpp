#ifndef NYUSHELL_H // include guard
#define NYUSHELL_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "cmd"
#include <sys/wait.h>
#include "ShellStatus.hpp"
#include "SubProcess.hpp"

using namespace std;


class NyuShell {
public:
	// command name to handler
    unordered_map<string, CmdBase*> mlt;
    // command handlers
    vector<CmdBase*> cmds;
    // shell status
    ShellStatus status;

    NyuShell();
    ~NyuShell();

    // wait all children process and main cpids list
    void waitUntilClear();
    // output prompt and parse the input
    vector<string> prompt(string& cmd);

    bool constrcutSubProcess(vector<vector<string>>& cmds, vector<int>& cleanUpList, vector<SubProcess*>& subs);

    // start the shell service
    void serve();
};




#endif 