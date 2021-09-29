#ifndef LSCMD_H // include guard
#define LSCMD_H

#include "CmdBase.hpp"

class LsCmd: public CmdBase
{
public:
	LsCmd();

	void execCmd(const vector<string>& args);

	~LsCmd();
};

#endif 