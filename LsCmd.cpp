#include "LsCmd.hpp"
#include <iostream>

using namespace std;

LsCmd::LsCmd()
{
	this->cmd = "ls";
}

LsCmd::~LsCmd()
{
	
}


void LsCmd::execCmd(const vector<string>& args)
{
	cout << "list files" << endl;
	
}