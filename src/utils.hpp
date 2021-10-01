#ifndef UTILS_H // include guard
#define UTILS_H

#include<iostream>
#include <vector>
#include <unistd.h>

using namespace std;

// get current working directory
string getMyCwd();

// split the working directory / cmd into vector
vector<string> splitStr(string s, string delimiter);

int changedir(string path);

// a cpp wrapper of execvp
pid_t execute(vector<string>& arglist);

inline void redirInput(string inputFile);
inline void redirOutput(string outputFile, bool bAppend);

#endif 