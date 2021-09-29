#ifndef CmdBase_H // include guard
#define CmdBase_H
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

class CmdBase {
public:
	string cmd;

	virtual void execCmd(const vector<string>& args) = 0;

	virtual ~CmdBase();
};

#endif 