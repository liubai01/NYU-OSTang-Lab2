#include "SubProcess.hpp"

#include <unistd.h>
#include "utils.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

SubProcess::SubProcess()
{
	inDp = -1;
	outDp = -1;

	parentJob = nullptr;
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