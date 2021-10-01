#include "NyuShell.hpp"
#include<iostream>
#include <unistd.h>
#include "utils.hpp"

using namespace std;

#define REGISTERSHELLCMD(X) cmds.push_back(new X())

NyuShell::NyuShell()
{
    // built-in command multiplexer initialization
    REGISTERSHELLCMD(CdCmd);

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
        
        // exec the built-in command
        auto f = mlt.find(args[0]);
        if (f != mlt.end())
        {
            f->second->execCmd(args);
        } else {
        	// exec the general id
            pid_t cpid = execute(args);
            // register in lists
            cpids.insert(cpid);
            waitUntilClear();

        }

    }
}

void NyuShell::waitUntilClear()
{
    while (true) {
        // by reference to https://stackoverflow.com/questions/279729/how-to-wait-until-all-child-processes-called-by-fork-complete
        int status;
        pid_t ret = wait(&status);
        cpids.erase(ret);
        if (ret == -1) {
            if (errno == ECHILD) break;
        } else {
            if (WEXITSTATUS(status) != 0 || !WIFEXITED(status)) {
                cerr << "pid " << ret << " failed" << endl;
                // exit(1);
            }
        }
    }
}