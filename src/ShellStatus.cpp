#include "ShellStatus.hpp"

ShellStatus::ShellStatus()
{
    activeProcessNum = 0;
    jobs.clear();
    subs.clear();
}


void ShellStatus::registerJob(Job& j)
{
    jobs.push_back(j);
    j.listElem = --jobs.end();
}

void ShellStatus::registerSubProcess(SubProcess& s, Job& j)
{
    s.parentJob = &j;
    ++activeProcessNum;
	subs.push_back(s);
}

void ShellStatus::deleteSubProcess(pid_t pid) 
{
    auto s = subs.begin();
    SubProcess sobj;

    while(s != subs.end())
    {
        if (s->pid == pid)
        {
            sobj = *s;
            if (--(sobj.parentJob->subnum) == 0)
            {
                jobs.erase(sobj.parentJob->listElem);
            }
            subs.erase(s);

            --activeProcessNum;
            break;
        }
        ++s;
    }
}

void ShellStatus::debugPrint()
{
    cout <<"=== Status Image ====" << endl;
    cout << "subnum: " << subs.size() << endl;
    for(auto& s: subs)
    {
        s.debugPrint();
    }
    cout << "jobnum: " << jobs.size() << endl;
    for(auto& j: jobs)
    {
        j.debugPrint();
    }
    cout << endl;
}

void ShellStatus::printJobs()
{
    int i = 0;
    for(auto& j: jobs)
    {
        cout << "[" << ++i << "] " << j.cmd << endl;
    }
}