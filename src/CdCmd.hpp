#ifndef CDCMD_H // include guard
#define CDCMD_H

#include "CmdBase.hpp"

class CdCmd: public CmdBase
{
public:
	CdCmd(ShellStatus* status);

	void execCmd(const vector<string>& args);

	~CdCmd();
};

#endif 