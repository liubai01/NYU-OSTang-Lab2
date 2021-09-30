#ifndef CDCMD_H // include guard
#define CDCMD_H

#include "CmdBase.hpp"

class CdCmd: public CmdBase
{
public:
	CdCmd();

	void execCmd(const vector<string>& args);

	~CdCmd();
};

#endif 