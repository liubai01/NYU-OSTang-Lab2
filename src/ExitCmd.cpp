#include "ExitCmd.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

ExitCmd::ExitCmd(ShellStatus* status):CmdBase(status)
{
	this->cmd = "exit";
}

ExitCmd::~ExitCmd()
{

}


void ExitCmd::execCmd(const vector<string>& args)
{
	if(args.size() > 1){
		cerr << "Error: invalid command" << endl;
	} else if (status->hasSuspendedJob)
	{
		cerr << "Error: there are suspended jobs" << endl;
	} else {
		exit(0);
	}
}