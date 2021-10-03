#include "JobsCmd.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

JobsCmd::JobsCmd(ShellStatus* status):CmdBase(status)
{
	this->cmd = "jobs";
}

JobsCmd::~JobsCmd()
{

}


void JobsCmd::execCmd(const vector<string>& args)
{
	if(args.size() > 1){
		cerr << "Error: invalid command" << endl;
	} else {
		status->printJobs();
	}
}