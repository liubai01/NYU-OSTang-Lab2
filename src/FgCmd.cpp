#include "FgCmd.hpp"
#include "utils.hpp"
#include <iostream>
#include <signal.h>

using namespace std;

FgCmd::FgCmd(ShellStatus* status):CmdBase(status)
{
	this->cmd = "fg";
}

FgCmd::~FgCmd()
{

}


void FgCmd::execCmd(const vector<string>& args)
{
	if(args.size() > 2 || args.size() == 1) {
		cerr << "Error: invalid command" << endl;
	} else {
		bool f = false;
		int idx = stoi(args[1]);
		for(auto& j: status->jobs) {
			if (j->idx == idx) {
				f = true;
				break;
			}
		}
		if (!f) {
			cerr << "Error: invalid job" << endl;
			return;
		}

		for(auto& s: status->subs)
		{
			if(!s->active && s->parentJob->idx == idx)
			{
				kill(s->pid, SIGCONT);
				++s->parentJob->activeSubNum;
			}
		}
		++status->activeJobNum;	
	}
	
}