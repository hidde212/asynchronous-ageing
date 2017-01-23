#ifndef MILS_RANDOMNUMBERS_H
#define MILS_RANDOMNUMBERS_H

#include <random>

long randomize();

// random integer [0,n]
int rn(const int&);

// random uniform [0,1)
double ru();

// random standard normal
double normal(const double &mean, const double &stddev);

// random binary
bool r2();

// random Poisson
int rpois(const double&);

// random exponential
double rexp(const double&);

// generate random index based on vector of int weights 
// may template this later...
int rindex(const std::vector<double> &w);

extern std::mt19937 rng;

#endif //MILS_RANDOMNUMBERS_H