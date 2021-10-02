#include "Job.hpp"

#include <unistd.h>
#include "utils.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

Job::Job()
{
	inDp = -1;
	outDp = -1;

	args.clear();
}

bool Job::setInDp(int dp)
{
	if (inDp != -1)
	{
		return false;
	}

	inDp = dp;
	return true;
}

bool Job::setOutDp(int dp)
{
	if (outDp != -1)
	{
		return false;
	}

	outDp = dp;
	return true;
}

void Job::exec(vector<int>& closeList)
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

string Job::getInfo()
{
	string ret;

	for (auto& a: args)
	{
		ret += a + " ";
	}

	return ret.substr(0, ret.length() - 1);
}

void Job::debugPrint()
{
    cout << "[" << args[0] << "] " << endl;
    for (int i = 1; i < args.size(); ++i) {
        cout << args[i] << endl;
    }

    if (outDp != -1) {
        cout << "output redirected" << endl;
    }

    if (inDp != -1) {
        cout << "input redirected" << endl;
    }
}