#include<iostream>
#include <unistd.h>
#include <vector>
#include "utils.hpp"

using namespace std;

int main() {

    // main loop
    for(;;)
    {
        string cmd;
        // current working directory stuff
        string cwd = getMyCwd();
        vector<string> cwdTokens = splitCwd(cwd, "/");
        string currDir = cwdTokens[cwdTokens.size() - 1];

        cout << "[" << "nyush " << currDir << "]" << "$: ";
        cin >> cmd;
    }

    return 0;
}