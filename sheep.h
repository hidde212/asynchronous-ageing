#ifndef MILS_OPZET_SHEEP_H
#define MILS_OPZET_SHEEP_H

#include <random>
#include "globals.h"
#include "randomnumbers.h"

//Class def:
class sheep{
public:
    sheep();									// Constructor called upon initialization

    //Set functions
    void setGen1(const double &setManual = 0.0);	// Set genes 1/2/3. If argument is given, gene value equals this argument.
    void setGen2(const double &setManual = 0.0);	// Else without argument, pulled from normal distribution with mean/stddev defined in globals.cpp
    void setGen3(const double &setManual = 0.0);
    void mutateGen1();							// Add mutations to traits
    void mutateGen2();
    void mutateGen3();

    //Get functions
    double getGen1() { return gen1; };									// Return values of genes 1/2/3
    double getGen2() { return gen2; };
    double getGen3() { return gen3; };
    int getAge()     { return age; };									// Return age of individual
    bool isAlive()   { return alive; };									// Check whether individual is alive or not - return TRUE or FALSE
    double getRepairResources() { return repairResources; };			// Return amount of resources individual allocates to damage prevention
    double getOffspringResources() { return offspringResources; };		// Return amount of resources individual allocates to producing offspring
    double getDamageTrait1()   { return damageTrait1; };				// Return amount of damage accumulated in trait 1
    double getDamageTrait2()   { return damageTrait2; };				// Return amount of damage accumulated in trait 2
    int getDeathCause() { return deathCause; };
    void advanceAge()     { ++age; };									// Advance age of individual by 1
    void addDamage();													// Add an amount of damage to individual, scaled by value of gen 1	
    void kill();														// Based on amount of accumulated damage, decide whether individual lives/dies

private:
    int age;					// Current age of individual
    double damageTrait1;		// Damage in component 1 an individual has accumulated over lifetime
    double damageTrait2;		// Damage in component 2 an inidivual has accumulated over lifetime
    bool alive;					// Whether alive, or not. TRUE or FALSE value
    int deathCause;

    double gen1;				// Allocation of resources towards repair
    double gen2;				// Allocation of repair-resources towards repair of damage1
    double gen3;				// Allocation of incoming damage when both damages are equal - goede beschrijving?

    double repairResources;		// Fraction of resources being appointed to repair
    double offspringResources;	// Fraction of resources used for reproduction
};
#endif //MILS_OPZET_SHEEP_H