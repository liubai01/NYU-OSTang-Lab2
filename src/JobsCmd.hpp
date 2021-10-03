#ifndef JOBSCMD_H // include guard
#define JOBSCMD_H

#include "CmdBase.hpp"

class JobsCmd: public CmdBase
{
public:
	JobsCmd(ShellStatus* status);

	void execCmd(const vector<string>& args);

	~JobsCmd();
};

#endif 