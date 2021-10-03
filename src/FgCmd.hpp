#ifndef FGCMD_H // include guard
#define FGCMD_H

#include "CmdBase.hpp"

class FgCmd: public CmdBase
{
public:
	FgCmd(ShellStatus* status);

	void execCmd(const vector<string>& args);

	~FgCmd();
};

#endif 