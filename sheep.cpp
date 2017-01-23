#include <stdexcept>
#include <cmath>
#include "sheep.h"


sheep::sheep() {						// Constructor called upon initialization -> 'birth'
    age = 0;
    damageTrait1 = 0.0001;				// Individual starts life with minimal amount of damage;
    damageTrait2 = 0.0001;               // this is above zero to prevent division by zero (in the addDamage function).
    alive = true;
    repairResources = gen1;				// Set resources invested in repair
    offspringResources = (1 - gen1);	// Remaining resources are invested in offspring
}

void sheep::setGen1(const double &setManual /* = 0 */) { // If default argument is given (setManual), gene value equals this argument. Else, value taken from normal distribution
    setManual ? gen1 = setManual : gen1 = normal(gen1Mean, gen1StdDev);

    if (gen1 < 0)		// Gen 1 is restricted to be between 0 and 1
        gen1 = 0;
    else if (gen1 > 1)
        gen1 = 1;
}

void sheep::setGen2(const double &setManual /* = 0 */) { // If default argument is given (setManual), gene value equals this argument. Else, value taken from normal distribution
    setManual ? gen2 = setManual : gen2 = normal(gen2Mean, gen2StdDev);
}

void sheep::setGen3(const double &setManual /* = 0 */) { // If default argument is given (setManual), gene value equals this argument. Else, value taken from normal distribution
    setManual ? gen3 = setManual : gen3 = normal(gen3Mean, gen3StdDev);
}

void sheep::kill() { // Calculate survival rate based on damage, and kill sheep accordingly
    if (ru() < extDeathRate) {									// External deathRate
        alive = false;
        deathCause = 0;
    }
    else if (ru() < 1 * exp(-rho1 * exp(-beta1 * damageTrait1))) {	// Internal death rate for damage 1 -> Gompertz law of mortality
        alive = false;
        deathCause = 1;
    }

    else if (ru() < 1 * exp(-rho2 * exp(-beta2 * damageTrait2))) {	// Internal death rate for damage 2 -> Gompertz law of mortality
        alive = false;
        deathCause = 2;
    }
}

void sheep::addDamage() {
    double baseDam = baseDamage * (1 - repairResources);				// Initial baseDamage scaled to amount of resources invested in damage prevention
    double damageAllocation;

    double relativeDamage = ((damageTrait1 - damageTrait2) / (damageTrait1 + damageTrait2));
    damageAllocation = (1 / (1 + exp(-gen2 * relativeDamage + gen3)));	//And allocation of resources accordingly

    damageTrait1 += damageAllocation * baseDam;						// Scale incoming damage to resources allocated
    damageTrait2 += (1 - damageAllocation) * baseDam;
}

void sheep::mutateGen1() { // Add mutation to gen 1. Restrictions: 0 > gen 1 > 1
    if (ru() < mutationRateGen1) {							// if mutationRateGen1 = 0.01, 1% of population mutates
        double mutation = normal(0, gen1MutationstdDev);	// Generate mutation. Standard deviation specified in globals.cpp 
        double mutatedGen1 = mutation + gen1;				// Add mutation to gene value .. 
        if (mutatedGen1 > 1)								// .. And apply restrictions
            gen1 = 1;
        else if (mutatedGen1 < 0)
            gen1 = 0;
        else
            gen1 = mutatedGen1;
    }
}

void sheep::mutateGen2() { // Add mutation to gen 2. No restriction to gene value
    if (ru() < mutationRateGen2) {
        double mutation = normal(0, gen2MutationstdDev);
        gen2 += mutation;
    }
}

void sheep::mutateGen3() { // Add mutation to gen 3
    if (ru() < mutationRateGen3) {
        double mutation = normal(0, gen3MutationstdDev);
        gen3 += mutation;
    }
}