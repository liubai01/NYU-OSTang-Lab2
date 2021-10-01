#include "NyuShell.hpp"

#include<iostream>
#include <unistd.h>
#include "utils.hpp"

using namespace std;

NyuShell::NyuShell()
{
    // built-in command multiplexer initialization
    cmds.push_back(new CdCmd());

    for (auto& c: cmds)
    {
        mlt[c->cmd] = c;
    }
}

NyuShell::~NyuShell()
{
    for (auto& c: cmds) {
        delete c;
    }
}

void NyuShell::serve()
{
	string cmd;
    vector<string> args;

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
}