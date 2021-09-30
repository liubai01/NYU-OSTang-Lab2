#include "CdCmd.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

CdCmd::CdCmd()
{
	this->cmd = "cd";
}

CdCmd::~CdCmd()
{

}


void CdCmd::execCmd(const vector<string>& args)
{
	if(args.size() == 1) {
		changedir("~/");
	} else {
		changedir(args[1]);
	}
	
}