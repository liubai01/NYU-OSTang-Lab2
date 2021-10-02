#include "ShellStatus.hpp"

void ShellStatus::registerSubProcess(SubProcess& s)
{
	subs.push_back(s);
}

void ShellStatus::deleteSubProcess(pid_t pid) 
{
    auto s = subs.begin();
    while(s != subs.end())
    {
        if (s->pid == pid)
        {
            subs.erase(s);
            break;
        }
    }
}

void ShellStatus::debugPrint()
{
    cout << subs.size() << endl;
    for(auto& s: subs)
    {
        s.debugPrint();
    }
}