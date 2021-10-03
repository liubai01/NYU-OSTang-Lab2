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

    // initialize status
    status.hasSuspendedJob = false;

    // disable some signal
    signal(SIGINT, [](int){});
    signal(SIGQUIT, [](int){});
    signal(SIGTERM, [](int){});
    signal(SIGSTOP, [](int){});
    signal(SIGTSTP, [](int){});
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
                s->pid = cpid;
                
                if (cpid == (pid_t) - 1)
                {
                    cerr << "Fork failed" << endl;
                    exit(1);
                }

                if (cpid == 0) {
                    s->exec(cleanUpList);
                }
                status.registerSubProcess(s, j);
            }
            for(auto& i: cleanUpList)
            {
                close(i);
            }

            status.registerJob(j);

            // status.debugPrint();
            status.printJobs();
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
    bool invalid = false;
    for (int i = 0; i < cmds.size(); ++i)
    {
        subs[i] = new SubProcess();
        args = cmds[i];
        auto ptr = args.begin();

        // check each arg to filter out file dir and pass to sub-process
        while(ptr != args.end())
        {
            // refer to: http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
            if (*ptr == ">")
            {
                if (ptr + 1 == args.end() || i + 1 < cmds.size())
                {
                    invalid = true;
                    break;
                }
                const char* outputFileC = (++ptr)->c_str();
                int out = open(outputFileC, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                subs[i]->setOutDp(out);
                cleanUpList.push_back(out);
            } else if (*ptr == ">>") {
                if (ptr + 1 == args.end() || i + 1 < cmds.size())
                {
                    invalid = true;
                    break;
                }
                const char* outputFileC = (++ptr)->c_str();
                int out = open(outputFileC, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                subs[i]->setOutDp(out);
                cleanUpList.push_back(out);
            } else if (*ptr == "<") {
                if (ptr + 1 == args.end() || i - 1 >= 0)
                {
                    invalid = true;
                    break;
                }
                const char* inputFileC = (++ptr)->c_str();
                int in = open(inputFileC, O_RDONLY);
                subs[i]->setInDp(in);
                cleanUpList.push_back(in);
            } else if (ptr->find("<") != string::npos || ptr->find(">") != string::npos) {
                invalid = true;
                break;
            } else {
                subs[i]->args.push_back(*ptr);
            }
            ++ptr;
        }

        // apply pipe
        if (i * 2 - 2 >= 0) {
            subs[i]->setInDp(pipes[i * 2 - 2]);
        }
        if (i * 2 + 1 < n) {
            subs[i]->setOutDp(pipes[i * 2 + 1]);
        }

        if (invalid)
        {
            break;
        }
    }

    if (invalid)
    {
        cerr << "Error: invalid command" << endl;
        return true;
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
    cout << "[nyush " << currDir << "]$: ";
    if(!getline(cin, cmd))
    {
        cout << endl;
        cin.clear();
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
    cout << "===wait loop begin===" << endl;
    while (this->status.activeJobNum) {
        // by reference to https://stackoverflow.com/questions/279729/how-to-wait-until-all-child-processes-called-by-fork-complete
        int status;
        cout << "activeJobNum: " << this->status.activeJobNum << endl;
        pid_t ret = waitpid(-1, &status, WCONTINUED | WUNTRACED);
        

        if (ret == -1) {
            // cout << "error" << endl;
            if (errno == ECHILD) break;
        } else {

            if (WIFSTOPPED(status))
            {
                cout << "stop: " << ret << endl;
                this->status.stopSubProcess(ret);
            }

            if (WIFCONTINUED(status))
            {
                cout << "cont: " << ret << endl;
                this->status.contSubProcess(ret);
            }

            if (WIFSIGNALED(status) || WIFEXITED(status))
            {
                cout << "delete: " << ret << endl;
                this->status.deleteSubProcess(ret);
            }

            // if (WEXITSTATUS(status) != 0 || !WIFEXITED(status)) {
            //     cerr << "pid " << ret << " failed" << endl;
            //     exit(1);
            // }
        }

    }
    cout << "===wait loop end===" << endl;
}
