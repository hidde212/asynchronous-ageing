#include <iostream>
#include <chrono>
#include <random>
#include "randomnumbers.h"

std::mt19937 rng;

long randomize() {
    static std::random_device rd{};
    auto seed = rd();
    std::cout << "Used seed: "<< seed << std::endl;
    rng.seed(seed);
    return seed;
}

// random integer {0,...,n} (including n)
int rn(const int &n)
{
    std::uniform_int_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d(rng, parm_t{ 0,n });
}

// random double [0,1)
double ru()
{
    std::uniform_real_distribution<> d{};
    return d(rng);
}

// random standard normal
double normal(const double &mean, const double &stddev)
{
    std::normal_distribution<> d{mean, stddev};
    return d(rng);
}

// random bernoulli {0,1}
bool r2()
{
    static std::bernoulli_distribution d{};
    return d(rng);
}

int rpois(const double &lambda)
{
    std::poisson_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d(rng, parm_t{lambda});
}

double rexp(const double &lambda)
{
    std::exponential_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d(rng, parm_t{ lambda });
}

int rindex(const std::vector<double> &w)
{
    std::discrete_distribution<> ri(begin(w), end(w));
    return(ri(rng));
}