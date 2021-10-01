#include "NyuShell.hpp"
#include<iostream>
#include <unistd.h>
#include "utils.hpp"

using namespace std;

#define REGISTERSHELLCMD(X) cmds.push_back(new X(&status))

NyuShell::NyuShell()
{
    // built-in command multiplexer initialization
    REGISTERSHELLCMD(CdCmd);
    REGISTERSHELLCMD(ExitCmd);

    for (auto& c: cmds)
    {
        mlt[c->cmd] = c;
    }

    // initialize status
    status.hasSuspendedJob = false;
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
        args = prompt();

        if (args.size() == 0)
        {
            continue;
        }
        
        // exec the built-in command
        auto f = mlt.find(args[0]);
        if (f != mlt.end())
        {
            f->second->execCmd(args);
        } else {
        	// exec outside commands

            pid_t cpid = execute(args);

            // register child's pid
            cpids.insert(cpid);
            status.hasSuspendedJob = cpids.size() > 0;

            waitUntilClear();
        }

    }
}

vector<string> NyuShell::prompt()
{
	vector<string> args;
	string cmd;

    // current working directory stuff
    string cwd = getMyCwd();
    vector<string> cwdTokens = splitStr(cwd, "/");
    string currDir = cwdTokens[cwdTokens.size() - 1];

    // prompt
    cout << "[nyush " << currDir << "]$: ";
    getline(cin, cmd);

    if (cmd.length() == 0)
    {
    	return {};
    }

    // parse the command
    args.clear();
    args = splitStr(cmd, " ");

    return args;
}

void NyuShell::waitUntilClear()
{
    while (true) {
        // by reference to https://stackoverflow.com/questions/279729/how-to-wait-until-all-child-processes-called-by-fork-complete
        int status;
        pid_t ret = wait(&status);
        cpids.erase(ret);
        this->status.hasSuspendedJob = cpids.size() > 0;

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
