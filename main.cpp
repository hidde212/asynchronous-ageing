#include <iostream>
#include <chrono>
#include <vector>
#include <exception>
#include <fstream>
#include <string>
#include <stdexcept>
#include "sheep.h"
#include "globals.h"
#include "randomnumbers.h"

//Function declaration:

std::vector<sheep> initiatePopulation(const unsigned long &popsize);														// Initiate population of size 'popSize'
void varyParameter(const double &first, const double &last, const double &delta, double &parameter, std::string prmName);	// Vary given parameters, and run a single cohort with this parameter until all indiv. are dead
void iterate(std::string outputFileName = "", double parameter = NULL, std::vector<sheep> vHerd = {} );						// Run a single cohort until all sheep are dead. No reproduction
void reproduceSheep(std::vector<sheep> &generation, const double &a);														// Allow sheep to reproduce
void simulate(const std::string &fileName);																					// Run multiple generations of sheep. reproduction and mutations allowed
void outputParams();																										// Create logfile.txt, containting all parameters from simulation

int main() {

    try {
        //Write paramaters to logfile - calls randomize function to determine seed
        outputParams();

        //Simulate 10 times per parameter combination
        simulate("Gomp+Gomp_simulation");
    }

    catch (std::exception &error) {
        std::cerr << error.what();
        exit(EXIT_FAILURE);
    }
}

// Function definitions:

std::vector<sheep> initiatePopulation(const unsigned long &popsize) {
    // Initiate the starting population, of size 'popSize'
    std::vector<sheep> generation(popsize);
    for (size_t i = 0; i < generation.size(); ++i) {
        sheep Sheep;
        Sheep.setGen1();		// Give newborn individual gene values for all three genes
        Sheep.setGen2();
        Sheep.setGen3();
        generation[i] = Sheep;	// Add sheep to generation vector
    }
    return generation;
}

void iterate(std::string outputFileName, double parameter, std::vector<sheep> vHerd) {
    // Build a starting cohort, and let simulation run until all sheep are dead. No reproduction / mutations

    if (vHerd.size() == 0) {						// If given vector is empty..
        vHerd = initiatePopulation(popSize);		//..initiate a new population, and proceed
    }
    std::string fileName;
    if (outputFileName == "") {						// If empty string (no argument given)
        fileName = "singleGen.csv";
    }
    else {											// Else if argument was given, make argument + value of parameter name of output file
        fileName = outputFileName + std::to_string(parameter) + ".csv";
    }

    std::ofstream initialGeneration(fileName);		// Open ofstream for data storage
    initialGeneration	<< "Time, " << "totalGen1, " << "totalGen2, " << "totalGen3," << "dAlive, "
                         << "Damage1Alive, " << "Damage2Alive, " << "NrDead, " << "Damage1Dead, " << "Damage2Dead" << std::endl;

    // Counters for statistics
    double gen1Total, gen2Total, gen3Total, Damage1Alive, Damage2Alive, Damage1Dead, Damage2Dead;
    int iAlive, iDead;
    int iTime = 0;

    do {
        gen1Total = gen2Total = gen3Total = Damage1Alive = Damage2Alive = Damage1Dead = Damage2Dead = 0.0;
        iAlive = iDead = 0;
#pragma omp parallel for schedule(dynamic) shared(iDead, iAlive, gen1Total, gen2Total, gen3Total, Damage1Alive, Damage2Alive, Damage1Dead, Damage2Dead)
        for (size_t i = 0; i < vHerd.size(); ++i) {				// Iterate over each sheep in population
            if (vHerd[i].isAlive()) {							// If alive..
                vHerd[i].addDamage();							// Add random small amount of damage
                vHerd[i].kill();								// And kill sheep according to their damage
                if (vHerd[i].isAlive()) {						// If alive, collect data
                    gen1Total += vHerd[i].getGen1();
                    gen2Total += vHerd[i].getGen2();
                    gen3Total += vHerd[i].getGen3();
                    Damage1Alive += vHerd[i].getDamageTrait1();
                    Damage2Alive += vHerd[i].getDamageTrait2();
                    ++iAlive;
                    vHerd[i].advanceAge();
                }
                else {											// Else if dead..
                    Damage1Dead += vHerd[i].getDamageTrait1();	// Gather damage information
                    Damage2Dead += vHerd[i].getDamageTrait2();
                    ++iDead;
                }
            }
        }
        // Output
        initialGeneration	<< iTime << ", " << gen1Total << ", " << gen2Total << ", " << gen3Total << ", " << iAlive << ", "
                             << Damage1Alive << ", " << Damage2Alive << ", "<< iDead << ", " << Damage1Dead << ", " << Damage2Dead << std::endl;

        std::cout << "Timestep: " << iTime << std::endl;

        ++iTime;
    } while (iAlive > 0 && iTime < maxGens);	    // End simulation if all sheep are dead,
    // or maximum nr. of simulations is reached
}

void varyParameter(const double &first, const double &last, const double &delta, double &parameter, std::string prmName) {
    // Vary a given parameter, and run multiple single cohorts until all are dead

    for (parameter = first; parameter < last; parameter += delta) {
        std::cout << "Trying " << prmName << " for value: " << parameter << std::endl;
        iterate(prmName, parameter);
    }
}

void reproduceSheep(std::vector<sheep> &generation, const double &alfa) {
    // Reproduce all sheep, then replace dead individuals with newborns

    std::vector<double> offspring(popSize);
    std::vector<int> deadSheep;

#pragma omp parallel for schedule(dynamic)
    for (size_t i = 0; i < generation.size(); ++i) {	// Determine number of offspring for each individual
        double tmp = generation[i].getOffspringResources();
        double dOffspring = (maxOffspring * tmp) / (alfa + tmp);
        if (dOffspring == 0)
            dOffspring = 0.0001;
        offspring[i] = dOffspring;
    }

    for (size_t i = 0; i < generation.size(); ++i) {	// Find dead sheep in generation, and store their position in the vector
        if (!generation[i].isAlive())
            deadSheep.push_back(i);
    }

#pragma omp parallel for schedule(dynamic)
    for (size_t j = 0; j < deadSheep.size(); ++j) {
        int parent = rindex(offspring);					// Pick parent using weighted lottery

        sheep Sheep;									// Create offspring, and give same gen1 and gen2 as parent..
        Sheep.setGen1(generation[parent].getGen1());
        Sheep.setGen2(generation[parent].getGen2());
        Sheep.setGen3(generation[parent].getGen3());
        Sheep.mutateGen1();							// .. and mutate possibly
        Sheep.mutateGen2();
        Sheep.mutateGen3();

        generation[deadSheep[j]] = Sheep;				// Replace dead sheep with their offspring
    }
}

void simulate(const std::string &fileName) {
    // Run multiple generations, reproduction and mutations included

    int iDead, iAlive;
    double dGen1, dGen2, dGen3, dDeadgen1, dDeadgen2, dDeadGen3, dDamage1, dDamage2, dDamage1Dead, dDamage2Dead, dAgeDead, dAgeAlive;	// Counters for statistics
    int iTime = 0;													// Nr of simulations to run

    std::vector<sheep> vHerd(popSize);								// Make a vector of size 'popSize'
    vHerd = initiatePopulation(popSize);							// And initialize a population, to be stored in vector

    // Open ofstream/datafile
    std::ofstream multipleGenerations("MGD_" + fileName + ".csv");
    multipleGenerations << "Generation," << "NrAlive," << "AgeAlive," << "Gen1," << "Gen2," << "Gen 3," << "Damage1," << "Damage2,"
                        << "Gen1Dead," << "Gen2Dead," << "Gen3Dead," << "Damage1Dead," << "Damage2Dead," << "NrDead," << "AgeDead" << std::endl;

    std::ofstream individualData("Individual_Data_" + fileName + ".csv");
    individualData << "Generation," << "Alive," << "Age," << "Damage1," << "Damage2," << "Gen1," << "Gen2," << "Gen 3," << "DeathCause" << std::endl;

    do {
        iDead = iAlive = 0, dGen1 = dGen2 = dGen3 = dDeadgen1 = dDeadgen2 = dDeadGen3 = dDamage1 = dDamage2 = dDamage1Dead = dDamage2Dead = dAgeDead = dAgeAlive = 0.0;
#pragma omp parallel for schedule(dynamic)
        for (size_t i = 0; i < vHerd.size(); ++i) {
            vHerd[i].addDamage();			// Add damage to sheep..
            vHerd[i].kill();				// .. and kill accordingly
            if (vHerd[i].isAlive()) {
                vHerd[i].advanceAge();		// If sheep survived, +1 to age
            }
        }
        if (iTime % 50 == 0) {				// Every fiftieth generation, gather statistics:
#pragma omp parallel for schedule(dynamic) shared(iDead, iAlive, dGen1, dGen2, dGen3, dDeadgen1, dDeadgen2, dDeadGen3, dDamage1, dDamage2, dDamage1Dead, dDamage2Dead, dAgeDead, dAgeAlive)
            for (size_t i = 0; i < vHerd.size(); ++i) {
                if (!vHerd[i].isAlive()) {							// If dead, collect info:
                    ++iDead;										// Gather number of dead indivuals
                    dAgeDead += vHerd[i].getAge();					// Gather total age
                    dDeadgen1 += vHerd[i].getGen1();				// Gather genotype information on deceased individuals
                    dDeadgen2 += vHerd[i].getGen2();
                    dDeadGen3 += vHerd[i].getGen3();
                    dDamage1Dead += vHerd[i].getDamageTrait1();		// Gather damage information on deceased individuals
                    dDamage2Dead += vHerd[i].getDamageTrait2();
                }

                else {												// if alive, gather info
                    ++iAlive;										// Gather total number of alive individuals
                    dAgeAlive += vHerd[i].getAge();					// Gather total age
                    dGen1 += vHerd[i].getGen1();					// Gather genotype information
                    dGen2 += vHerd[i].getGen2();
                    dGen3 += vHerd[i].getGen3();
                    dDamage1 += vHerd[i].getDamageTrait1();			// Gather damage information
                    dDamage2 += vHerd[i].getDamageTrait2();
                }
            }
            // Output:
            multipleGenerations << iTime << "," << iAlive << "," << dAgeAlive << "," << dGen1 << "," << dGen2 << "," << dGen3 << "," << dDamage1 << "," << dDamage2 << ","			// Info on sheep alive
                                << dDeadgen1 << "," << dDeadgen2 << "," << dDeadGen3 << "," << dDamage1Dead << "," << dDamage2Dead << "," << iDead << "," << dAgeDead << std::endl;	// Info on dead sheep																																														
        }
        // Store info on every sheep every 5000th generation
        if (iTime % 5000 == 0) {
            for (size_t i = 0; i < vHerd.size(); ++i) {
                individualData << iTime << ", " << vHerd[i].isAlive() << ", " << vHerd[i].getAge() << ", " << vHerd[i].getDamageTrait1() << ", " << vHerd[i].getDamageTrait2()
                               << ", " << vHerd[i].getGen1() << ", " << vHerd[i].getGen2() << ", " << vHerd[i].getGen3() << ", " << vHerd[i].getDeathCause() << std::endl;
            }
        }

        reproduceSheep(vHerd, alfa);						// Reproduce sheep to fill up place of dead individuals

        ++iTime;
        if(iTime % 50 == 0)
            std::cout << "Simulating generation: " << iTime << std::endl;

        if (iTime == maxGens) {								// Let last generation continue until all are dead.
            std::cout << "Simulating last generation" << std::endl;
            iterate("LastGen" + fileName, NULL, vHerd);     // Nasty
        }
    } while (iTime < maxGens);
}

void outputParams(){
    std::ofstream ofParam("logfile.txt");
    ofParam << "//Global variables " << std::endl
            << "Seed =             "	<< randomize()				<< std::endl << std::endl
            << "popSize =          "	<< popSize					<< "		//(Initial) generation size " << std::endl
            << "intDeathRate =     "	<< intDeathRate				<< "		//Chance to die (lower is higher survivability); intrinsic death rate. Between 0 and 1. " << std::endl
            << "extDeathRate =     "	<< extDeathRate				<< "		//Fraction individuals who die each timestep, extrinsic death. Between 0 and 1. " << std::endl
            << "mutationRateGen1 = "	<< mutationRateGen1			<< "		//Chance of mutating genotype 1 (0.01 = 1%) " << std::endl
            << "mutationRateGen2 = "	<< mutationRateGen2			<< "        //Chance of mutating genotype 2 (0.01 = 1%) " << std::endl
            << "maxGens =          "	<< maxGens					<< "        //Maximum amount of generations allowed per simulation " << std::endl
            << "maxOffspring =     "	<< maxOffspring				<< "		//Maximum amount of offspring per individual " << std::endl
            << "alfa =             "	<< alfa						<< "		//Conversion factor for dependency offspring resources <-> number of offspring " << std::endl
            << "beta1 =            "    << beta1                    << "        //Factor (steepness) in Gompertz's law of mortality for damage of trait 1 " << std::endl
            << "beta2 =            "    << beta2                    << "        //Factor (steepness) in Gompertz's law of mortality for damage of trait 2 " << std::endl
            << "rho1 =             "    << rho1                     << "        //Factor (y-axis intersect) in Gompertz's law of mortality for damage of trait 1" << std::endl
            << "rho2 =             "    << rho2                     << "        //Factor (y-axis intersect) in Gompertz's law of mortality for damage of trait 2" << std::endl << std::endl

            << "//Genotype initialisation                   0 < Gen1 < 1 ;;; -inf < Gen2 < +inf ;;; -inf < Gen3 < +inf " << std::endl
            << "gen1Mean =         "    << gen1Mean					<< "		//Mean for constructing genotype 1 from normal distribution " << std::endl
            << "gen1Stddev =       "    << gen1StdDev				<< "		//Standard deviation for constructing genotype 1 from normal distribution " << std::endl
            << "gen2Mean =         "    << gen2Mean					<< "		//Mean for constructing genotype 1 from normal distribution " << std::endl
            << "gen2Stddev =       "    << gen2StdDev				<< "		//Standard deviation for constructing genotype 1 from normal distribution " << std::endl
            << "gen3Mean =         "    << gen3Mean					<< "		//Mean for constructing gene 3 from normal distribution" << std::endl
            << "gen3Stddev =       "    << gen3StdDev				<< "		//Standard deviation for constructing genotype 3 from normal distribution" << std::endl << std::endl

            << "//Mutation parameters:" << std::endl
            << "gen1MutateStddev = "	<< gen1MutationstdDev		<< "		//Standard deviation for mutations " << std::endl
            << "gen2MutateStddev = "	<< gen2MutationstdDev		<< "		//Average value of gen 2, when taken from normal distribution " << std::endl;

}