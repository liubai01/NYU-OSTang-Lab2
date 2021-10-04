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

	parentJob = nullptr;
	active = true;
	first = false;
	args.clear();
}

bool SubProcess::setInDp(int dp)
{
	if (inDp != -1)
	{
		return false;
	}

	inDp = dp;
	return true;
}

bool SubProcess::setOutDp(int dp)
{
	if (outDp != -1)
	{
		return false;
	}

	outDp = dp;
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

    // apply pipe
    if (inDp != -1)
    {
        dup2(inDp, 0);
    }

    if (outDp != -1)
    {
        dup2(outDp, 1);
    }

    for (auto& i: closeList)
    {
        close(i);
    }

    // execuate
    execute(args);
  	exit (1);
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