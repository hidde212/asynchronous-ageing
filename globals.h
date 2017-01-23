#ifndef MILS_WEEK_2_GLOBALS_H
#define MILS_WEEK_2_GLOBALS_H

//Global variables
extern unsigned long popSize;		// (Initial) generation size
extern double intDeathRate;		    // Chance to die (lower is higher survivability); intrinsic death rate.
extern double extDeathRate;			// Fraction individuals who die each timestep, extrinsic death
extern int maxGens;					// Maximum amount of generation allowed per simulation
extern double maxOffspring;			// Max nr. of offspring allowed per indvidual
extern double alfa;					// Dependency resources invested in offspring <-> actual offspring
extern double beta1;				// Factor (steepness) in Gompertz's law of mortality for damage of trait 1
extern double beta2;				// Factor (steepness) in Gompertz's law of mortality for damage of trait 2
extern double rho1;                 // Factor (y-axis intersect) in Gompertz's law of mortality for damage of trait 1
extern double rho2;                 // Factor (y-axis intersect) in Gompertz's law of mortality for damage of trait 2
extern double phi;                  // Factor in linear mortality curve
extern double baseDamage;           // Standard amount of damage added per timestep before allocation of repair/offspring resources


extern double gen1Mean;				// Mean for constructing genotype 1 from normal distribution
extern double gen1StdDev;			// Standard deviation for constructing genotype 1 from normal distribution
extern double gen2Mean;				// Mean for constructing genotype 1 from normal distribution
extern double gen2StdDev;			// Standard deviation for constructing genotype 1 from normal distribution
extern double gen3Mean;
extern double gen3StdDev;

extern double mutationRateGen1;		// Rate of mutation for gen 1
extern double mutationRateGen2;		// Rate of mutation for gen 2
extern double mutationRateGen3;		// Rate of mutation for gen 3

extern double gen1MutationstdDev;	// Standard deviation for mutation in gen 1
extern double gen2MutationstdDev;	// Standard deviation for mutation in gen 2
extern double gen3MutationstdDev;	// Standard deviation for mutation in gen 3

#endif //MILS_WEEK_2_GLOBALS_H