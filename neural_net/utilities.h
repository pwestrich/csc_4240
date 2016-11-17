
// header fules for utilities.cpp

#ifndef UTILITIES_H
#define UTILITIES_H

#include <chrono>
#include <random>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::default_random_engine;
using std::uniform_real_distribution;

//calculates a 2-d array's index
int index(const int row, const int col, const int numCols);

//retuens a random double between the two given numbers
double getRandomDouble(const double &lowBound, const double &highBound);

//splits a string into substrings given a delimiter
vector<string> explode(const string &delimiter, const string &str);

#endif
