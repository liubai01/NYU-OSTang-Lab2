#ifndef Job_H // include guard
#define Job_H

#include<vector>
#include<iostream>
#include<string>

using namespace std;


class Job {
public:
    int inDp;
    int outDp;
    vector<string> args;

    Job();

    void exec(vector<int>& closeList);

    string getInfo();
    void debugPrint();

    bool setInDp(int dp);
    bool setOutDp(int dp);
};

#endif 