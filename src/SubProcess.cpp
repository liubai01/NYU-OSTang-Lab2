#include "SubProcess.hpp"

#include <unistd.h>
#include "utils.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

SubProcess::SubProcess()
{
	inDp = -1;
	outDp = -1;

	inDpMode = OTHERF;
	outDpMode = OTHERF;

	parentJob = nullptr;
	active = true;
	first = false;
	args.clear();
}

bool SubProcess::setInDp(int dp, string filePath, int mode)
{
	if (inDp != -1)
	{
		return false;
	}

	inDp = dp;
	inPath = filePath;
	inDpMode = mode;
	return true;
}

bool SubProcess::setOutDp(int dp, string filePath, int mode)
{
	if (outDp != -1)
	{
		return false;
	}

	outDp = dp;
	outPath = filePath;
	outDpMode = mode;
	return true;
}

void SubProcess::exec(vector<int>& closeList)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGSTOP, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);

    // refer to: https://drustz.com/posts/2015/09/29/step-by-step-shell2/
    // signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    string cmd = args[0];
    bool containSlash = cmd.find("/") != string::npos;

    if (!containSlash) {
        if (checkIfExists("/bin/" + cmd)) {
            // args[0] = "/bin/" + cmd;
        } else if (checkIfExists("/usr/bin/" + cmd)) {
            // args[0] = "/usr/bin/" + cmd;
        } else {
            cerr << "Error: invalid program" << endl;
            exit(1);
        }
    } else {
        if (!checkIfExists(cmd))
        {
            cerr << "Error: invalid program" << endl;
            exit(1);
        }
    }

    // apply pipe
    if (inDp != -1)
    {
	   if (inDpMode == READF)
        {
            if (!checkIfExists(inPath))
            {
                cerr << "Error: invalid file" << endl;
                for (auto& i: closeList)
                {
                    close(i);
                }
                exit(1);
            }
            const char* inputFileC = inPath.c_str();
            inDp = open(inputFileC, O_RDONLY);
            closeList.push_back(inDp);
        }
        dup2(inDp, 0);
    }

    if (outDp != -1)
    {
        if (outDpMode == WRITEF)
        {
            const char* outputFileC = outPath.c_str();
            outDp = open(outputFileC, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            closeList.push_back(outDp);
        } else if (outDpMode == APPENDF) {
            const char* outputFileC = outPath.c_str();
            outDp = open(outputFileC, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            closeList.push_back(outDp);
        }
        dup2(outDp, 1);
    }

    for (auto& i: closeList)
    {
        close(i);
    }

    // execuate
    execute(args);
  	exit(1);
}

string SubProcess::getInfo()
{
	string ret;

	for (auto& a: args)
	{
		ret += a + " ";
	}

	return ret.substr(0, ret.length() - 1);
}

void SubProcess::debugPrint()
{
    cout << "[" << args[0] << "] ";
    for (int i = 1; i < args.size(); ++i) {
        cout << args[i] << " ";
    }
    cout << endl;

}