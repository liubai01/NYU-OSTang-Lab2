#include "ShellStatus.hpp"

ShellStatus::ShellStatus()
{
    activeJobNum = 0;
    jobs.clear();
    subs.clear();
}

ShellStatus::~ShellStatus()
{
    for(auto& j : jobs)
    {
        delete(j);
    }

    for(auto& s: subs)
    {
        delete(s);
    }
}


void ShellStatus::registerJob(Job* j)
{
    jobs.push_back(j);
    ++activeJobNum;
}

void ShellStatus::registerSubProcess(SubProcess* s, Job* j)
{
    s->parentJob = j;

    j->lastPid = s->pid;
    ++j->aliveSubNum;
    ++j->activeSubNum;

	subs.push_back(s);
}

void ShellStatus::stopSubProcess(pid_t pid)
{
    auto s = subs.begin();

    while(s != subs.end())
    {
        SubProcess* sptr = *s;
        if (sptr->pid == pid)
        {
            --(sptr->parentJob->activeSubNum);
            cout << "stop_activeSubProcess: " << sptr->parentJob->activeSubNum << endl;

            if (sptr->parentJob->activeSubNum == 0)
            {
                --activeJobNum;
            }
            break;
        }
        ++s;
    }
}

void ShellStatus::contSubProcess(pid_t pid)
{
    auto s = subs.begin();
    while(s != subs.end())
    {
        SubProcess* sptr = *s;
        if (sptr->pid == pid)
        {   
            ++(sptr->parentJob->activeSubNum);
        if (sptr->pid == pid)
            cout << "cont_activeSubProcess: " << sptr->parentJob->activeSubNum << endl;

            if (sptr->parentJob->activeSubNum == 1) {
                cout << "++JobNum" << endl;
                ++activeJobNum;
            }
            break;
        }
        ++s;
    }
}

void ShellStatus::deleteSubProcess(pid_t pid) 
{
    auto s = subs.begin();

    while(s != subs.end())
    {
        SubProcess* sptr = *s;
        if (sptr->pid == pid)
        {
            --(sptr->parentJob->activeSubNum);
            --(sptr->parentJob->aliveSubNum);

            if (sptr->parentJob->aliveSubNum == 0)
            {
                for (auto j = jobs.begin(); j != jobs.end(); ++j)
                {
                    Job* jptr = *j;
                    if (jptr->lastPid = sptr->parentJob->lastPid)
                    {
                        jobs.erase(j);
                        delete jptr;
                        break;
                    }
                }
                --activeJobNum;
            } else if (sptr->parentJob->activeSubNum == 0) {
                --activeJobNum;
            }
            subs.erase(s);
            delete sptr;

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
        s->debugPrint();
    }
    cout << "jobnum: " << jobs.size() << endl;
    for(auto& j: jobs)
    {
        j->debugPrint();
    }
    cout << endl;
}

void ShellStatus::printJobs()
{
    int i = 0;
    for(auto& j: jobs)
    {
        cout << "[" << ++i << "] " << j->cmd << endl;
    }
}