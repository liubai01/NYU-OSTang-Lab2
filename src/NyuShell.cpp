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
    vector<string> tokens;

	// main loop
    for(;;)
    {
        tokens = prompt();

        if (tokens.size() == 0)
        {
            continue;
        }
        
        // exec the built-in command
        auto f = mlt.find(tokens[0]);
        if (f != mlt.end())
        {
            f->second->execCmd(tokens);
        } else {
            // check whether there are pipes
            vector<vector<string>> cmds = splitTokens(tokens, "|");
            vector<string> args;

            int n = (cmds.size() - 1) * 2;
            int pipes[n];
            for (int i = 0; i < n; ++i)
            {
                pipe(pipes + i * 2);
            }

            // exec outside commands
            for (int i = 0; i < cmds.size(); ++i)
            {
                args = cmds[i];

                pid_t cpid = fork();

                if (cpid == 0)
                {
                    // apply pipe
                    if (i * 2 - 2 >= 0) {
                        dup2(pipes[i * 2 - 2], 0);
                    }
                    if (i * 2 + 1 < n) {
                        dup2(pipes[i * 2 + 1], 1);
                    }
                    for (int j = 0; j < n; ++j)
                    {
                        close(pipes[j]);
                    }
                    // parse input/output file redirection
                    parseRedirFile(args);
                    // execuate
                    execute(args);
                    // should not reach here
                }
                // register child's pid
                cpids.insert(cpid);
                status.hasSuspendedJob = cpids.size() > 0;
            }
            for (int j = 0; j < n; ++j)
            {
                close(pipes[j]);
            }

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
