#include "FgCmd.hpp"
#include "utils.hpp"
#include <iostream>

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
	if(args.size() > 2) {
		cerr << "Error: invalid command" << endl;
	} else if (args.size() == 1) {
		// status.jobs.front();
	}
	
}