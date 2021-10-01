#ifndef CMDBASE_H // include guard
#define CMDBASE_H
#include <vector>
#include <algorithm>
#include <iterator>
#include "ShellStatus.hpp"

using namespace std;


class CmdBase {
public:
	ShellStatus* status;
	string cmd;

	CmdBase(ShellStatus* status) : status(status) {};

	virtual void execCmd(const vector<string>& args) = 0;

	virtual ~CmdBase();
};

#endif 