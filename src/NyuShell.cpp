#include "NyuShell.hpp"
#include<iostream>
#include <unistd.h>
#include "utils.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "Job.hpp"

using namespace std;

#define REGISTERSHELLCMD(X) cmds.push_back(new X(&status))

NyuShell::NyuShell()
{
    // built-in command multiplexer initialization
    REGISTERSHELLCMD(CdCmd);
    REGISTERSHELLCMD(ExitCmd);
    REGISTERSHELLCMD(JobsCmd);
    REGISTERSHELLCMD(FgCmd);

    for (auto& c: cmds)
    {
        mlt[c->cmd] = c;
    }

    // disable some signal
    ppid = getpid();
    setpgid (ppid, ppid);
    foreground = STDIN_FILENO;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    // refer to: https://drustz.com/posts/2015/09/29/step-by-step-shell2/
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);


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
    string cmd;

	// main loop
    for(;;)
    {

        tokens = prompt(cmd);


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
            int pgid = -1;
            vector<vector<string>> cmds = splitTokens(tokens, "|");

            vector<SubProcess*> subs(cmds.size());
            vector<int> cleanUpList;

            Job* j = new Job();
            j->cmd = cmd;

            if (constrcutSubProcess(cmds, cleanUpList, subs))
            {
                continue;
            }

            for (auto& s: subs)
            {
                pid_t cpid = fork();

                if (cpid == (pid_t) - 1)
                {
                    // cerr << "Fork failed" << endl;
                    exit(1);
                }

                if (pgid == -1)
                {
                    s->first = true;
                    pgid = cpid;
                }
                s->pid = cpid;
                s->pgid = pgid;

                if (cpid == 0) {
                    tcsetpgrp(foreground, pgid);
                    setpgid(0, pgid);
                    s->exec(cleanUpList);
                    // should not reach here
                } else {
                    if (s->first)
                    {
                        setpgid(cpid, pgid);
                        tcsetpgrp(foreground, cpid);
                    } else {
                        setpgid(cpid, pgid);
                    }

                    status.registerSubProcess(s, j);
                }

            }
            for(auto& i: cleanUpList)
            {
                close(i);
            }

            status.registerJob(j);

            // status.debugPrint();
            // status.printJobs();
            // cout << "After" << endl;
            // status.debugPrint();
           
        }
        waitUntilClear();

    }
}


bool NyuShell::constrcutSubProcess(vector<vector<string>>& cmds, vector<int>& cleanUpList, vector<SubProcess*>& subs)
{
    // construct pipes
    int n = (cmds.size() - 1) * 2;
    int pipes[n];
    for (int i = 0; i < n / 2; ++i)
    {
        pipe(pipes + i * 2);

        cleanUpList.push_back(pipes[i * 2]);
        cleanUpList.push_back(pipes[i * 2 + 1]);
    }
    vector<string> args;
    // construct sub-process
    for (int i = 0; i < cmds.size(); ++i)
    {
        subs[i] = new SubProcess();
        args = cmds[i];
        if (args.size() == 0 || args.size() == 1 && args[0].size() == 0)
        {
            cerr << "Error: invalid command" << endl;
            return true;
        }
        auto ptr = args.begin();

        bool shouldTerm = false;

        // check each arg to filter out file dir and pass to sub-process
        while(ptr != args.end())
        {
            // refer to: http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
            if (*ptr == ">")
            {
                // output logic
                shouldTerm = true;
                if (ptr + 1 == args.end() || i + 1 < cmds.size())
                {
                    cerr << "Error: invalid command" << endl;
                    return true;
                }
                if(!subs[i]->setOutDp(0, *++ptr, WRITEF))
                {
                    cerr << "Error: invalid command" << endl;
                    return true;
                }
            } else if (*ptr == ">>") {
                // append logic
                shouldTerm = true;
                if (ptr + 1 == args.end() || i + 1 < cmds.size())
                {
                    cerr << "Error: invalid command" << endl;
                    return true;
                }
                if (!subs[i]->setOutDp(0, *++ptr, APPENDF))
                {
                    cerr << "Error: invalid command" << endl;
                    return true;
                }
            } else if (*ptr == "<") {
                // read logic
                shouldTerm = true;
                if (ptr + 1 == args.end() || i - 1 >= 0)
                {
                    cerr << "Error: invalid command" << endl;
                    return true;
                }
                if(!subs[i]->setInDp(0, *++ptr, READF))
                {
                    cerr << "Error: invalid command" << endl;
                    return true;
                }
            } else if (ptr->find("<") != string::npos || ptr->find(">") != string::npos) {
                cerr << "Error: invalid command" << endl;
                return true;
            } else {
                if (shouldTerm)
                {
                    cerr << "Error: invalid command" << endl;
                    return true;
                }
                subs[i]->args.push_back(*ptr);
            }
            ++ptr;
        }

        // apply pipe
        if (i * 2 - 2 >= 0) {
            subs[i]->setInDp(pipes[i * 2 - 2], "", OTHERF);
        }
        if (i * 2 + 1 < n) {
            subs[i]->setOutDp(pipes[i * 2 + 1], "", OTHERF);
        }

    }


    return false;
}

vector<string> NyuShell::prompt(string& cmd)
{
	vector<string> args;

    // current working directory stuff
    string cwd = getMyCwd();
    vector<string> cwdTokens = splitStr(cwd, "/");
    string currDir = cwdTokens[cwdTokens.size() - 1];

    // prompt
    cout << "[nyush " << currDir << "]$ ";

    if(!getline(cin, cmd))
    {
        cout << endl;
        exit(0);
    }

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
    while (this->status.activeJobNum) {
        // cout << "active num: " << this->status.activeJobNum << endl;
        // by reference to https://stackoverflow.com/questions/279729/how-to-wait-until-all-child-processes-called-by-fork-complete
        int status;
        pid_t ret = waitpid(-1, &status, WCONTINUED | WUNTRACED);
        

        if (ret == -1) {
            // cout << "error" << endl;
            if (errno == ECHILD) break;
        } else {

            if (WIFSTOPPED(status))
            {  
                // cout << "stop: " << ret << endl;
                this->status.stopSubProcess(ret);
            }

            if (WIFCONTINUED(status))
            {
                // cout << "cont: " << ret << endl;
                this->status.contSubProcess(ret);
            }

            if (WIFSIGNALED(status) || WIFEXITED(status))
            {
                // cout << "exit: " << ret << endl;
                this->status.deleteSubProcess(ret);
                
            }

            // if (WEXITSTATUS(status) != 0 || !WIFEXITED(status)) {
            //     cerr << "pid " << ret << " failed" << endl;
            //     exit(1);
            // }
        }

    }
    if (tcsetpgrp(foreground, ppid))
    {
        // cerr << "failed" << endl;
    }
}
