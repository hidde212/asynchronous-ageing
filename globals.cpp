#include "globals.h"

//Global variables
unsigned long popSize = 10000;			// (Initial) generation size
double intDeathRate = 0.5;		    	// Chance to die (lower is higher survivability); intrinsic death rate.
double extDeathRate = 0.0001;			// Fraction individuals who die each timestep, extrinsic death
int maxGens = 50000;					// Maximum amount of generation allowed per simulation
double maxOffspring = 3.0;				// Max number of offspring allowed per individual
double alfa = 0.4;						// Conversion rate for dependency resources invested in offspring <-> actual offspring
double beta1 = 2.0;			        	// Factor (steepness) in Gompertz's law of mortality for damage of trait 1
double beta2 = 2.0;               		// Factor (steepness) in Gompertz's law of mortality for damage of trait 2
double rho1 = 5.0;                      // Factor (y-axis intersect) in Gompertz's law of mortality for damage of trait 1
double rho2 = 15.0;                     // Factor (y-axis intersect) in Gompertz's law of mortality for damage of trait 2
//double phi = 0.33;                      // Steepness of linear mortality curve
double baseDamage = 0.1;                // Standard amount of damage added per timestep before allocation of repair/offspring resources

double gen1Mean = 0.5;					// Mean for constructing gene 1 from normal distribution
double gen1StdDev = 0.05;				// Standard deviation for constructing genotype 1 from normal distribution
double gen2Mean = 0.0;					// Mean for constructing gene 2 from normal distribution
double gen2StdDev = 0.1;				// Standard deviation for constructing genotype 2 from normal distribution
double gen3Mean = 0.0;					// Mean for constructing gene 3 from normal distribution
double gen3StdDev = 0.1;				// Standard deviation for constructing genotype 3 from normal distribution

double mutationRateGen1 = 0.01;			// Rate of mutation for gen 1
double mutationRateGen2 = 0.05;			// Rate of mutation for gen 2
double mutationRateGen3 = 0.05;			// Rate of mutation for gen 3

double gen1MutationstdDev = 0.05;		// Standard deviation for mutation in gen 1
double gen2MutationstdDev = 0.5;		// Standard deviation for mutation in gen 2
double gen3MutationstdDev = 0.5;		// Standard deviation for mutation in gen 3