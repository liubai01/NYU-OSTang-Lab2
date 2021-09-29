#ifndef UTILS_H // include guard
#define UTILS_H

#include<iostream>
#include <unistd.h>
#include <vector>

using namespace std;

// get current working directory
string getMyCwd();

// split the working directory into vector
vector<string> splitStr(string s, string delimiter);

#endif 