#include "Job.hpp"

Job::Job()
{
    aliveSubNum = 0;
    activeSubNum = 0;

    lastPid = -1;
}

void Job::debugPrint()
{
	cout << "[" << cmd << "]: "; 
	cout << "alive: " << aliveSubNum << ", ";
	cout << "active: " << activeSubNum;
	cout << endl;
}