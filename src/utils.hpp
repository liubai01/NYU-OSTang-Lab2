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

vector<vector<string>> splitTokens(vector<string>& tokens, string delimiter);

int changedir(string path);

// a cpp wrapper of execvp
void execute(vector<string>& arglist);

// check whether exists
bool checkIfExists(const string& path);

#endif 