#ifndef NYUSHELL_H // include guard
#define NYUSHELL_H

#include <vector>
#include <unordered_map>
#include "cmd"

using namespace std;


class NyuShell {
public:
	// command name to handler
    unordered_map<string, CmdBase*> mlt;
    // command handlers
    vector<CmdBase*> cmds;

    NyuShell();
    ~NyuShell();

    void serve();
};


#endif 