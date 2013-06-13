/*
 * Random.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: cws
 */

#include "Random.h"

Random& Random::getInstance(int seed) {
    // The only instance
    // Guaranteed to be lazy initialized
    // Guaranteed that it will be destroyed correctly
    static Random instance(seed);
    return instance;
}

double Random::getRandom() {
	double randomNumber = ( (double)std::rand() / (double)RAND_MAX );
	return randomNumber;
}

size_t Random::getRandMax() {
	return RAND_MAX;
}

void Random::resetSeed(int seed) {
	// std::cout << "Resetting seed with value " << seed << "\n";
    std::srand(seed);
    _seed = seed;

}

int Random::getSeed() {
	return _seed;
}

Random::Random(int seed) {
	//TODO: Is the generator REALLY only initialized once?
	// std::cout << "Initializing random number generator with seed " << seed << "\n";
    std::srand(seed);
    _seed = seed;
}

Random::Random(){
	// std::cerr << "Random() should have not been called";
	_seed = -1; // should never be called
}
