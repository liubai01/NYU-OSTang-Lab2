#include "CdCmd.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

CdCmd::CdCmd(ShellStatus* status):CmdBase(status)
{
	this->cmd = "cd";
}

CdCmd::~CdCmd()
{

}


void CdCmd::execCmd(const vector<string>& args)
{
	if(args.size() == 1 || args.size() > 2) {
		cerr << "Error: invalid command" << endl;
	} else {
		if (changedir(args[1]) == -1)
		{
			cerr << "Error: invalid directory" << endl;
		}
	}
	
}