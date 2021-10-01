#ifndef UTILS_H // include guard
#define UTILS_H

#include<iostream>
#include <vector>
#include <unistd.h>

using namespace std;

// get current working directory
string getMyCwd();

// split the working directory into vector
vector<string> splitStr(string s, string delimiter);

int changedir(string path);

pid_t execute(vector<string>& arglist);

#endif 