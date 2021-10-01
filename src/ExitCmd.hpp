#ifndef EXITCMD_H // include guard
#define EXITCMD_H

#include "CmdBase.hpp"

class ExitCmd: public CmdBase
{
public:
	ExitCmd(ShellStatus* status);

	void execCmd(const vector<string>& args);

	~ExitCmd();
};

#endif 