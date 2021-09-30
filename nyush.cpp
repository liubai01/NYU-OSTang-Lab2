#include<iostream>
#include <unistd.h>
#include <vector>
#include <unordered_map>
#include "utils.hpp"
#include "cmd"

using namespace std;

int main() {
    string cmd;
    vector<string> args;

    // built-in command multiplexer initialization
    unordered_map<string, CmdBase*> mlt;
    vector<CmdBase*> cmds = {new CdCmd()};

    for (auto& c: cmds)
    {
        mlt[c->cmd] = c;
    }

    // main loop
    for(;;)
    {
        // current working directory stuff
        string cwd = getMyCwd();
        vector<string> cwdTokens = splitStr(cwd, "/");
        string currDir = cwdTokens[cwdTokens.size() - 1];

        // prompt
        cout << "[nyush " << currDir << "]$: ";
        getline(cin, cmd);

        // parse the command
        args.clear();
        if (cmd.size() == 0)
        {
            continue;
        }
        args = splitStr(cmd, " ");
        
        // exec the command
        auto f = mlt.find(args[0]);
        if (f != mlt.end())
        {
            f->second->execCmd(args);
        } else {
            execute(args);
        }

    }

    for (auto& c: cmds) {
        delete c;
    }

    return 0;
}