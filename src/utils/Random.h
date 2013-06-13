/*
 * Random.h
 *
 *  Created on: Nov 30, 2012
 *      Author: cws
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <cstdlib>
#include <iostream>
#include <ctime>

/**
 * Provides a random number generator for
 * uniformly distributed random number in (0, 1).
 * The class is implemented as a Singleton to avoid
 * multiple seeds.
 *
 */
class Random{
public:
	/**
	 * Get the singleton instance of the random number generator.
	 * Is per default initialized with the time of first call
	 */
	static Random& getInstance(int seed = time(NULL) );

	/**
	 * get a uniformly distributed random number in the interval (0,1)
	 */
	double getRandom();

	/**
	 * For testing purposes.
	 */
	size_t getRandMax();

	/**
	 * Explicitly creates a new instance with a particular seed
	 * contradicts the Singleton idea as the creation of a new instance is enforced.
	 * May be used to reproduce results or for testing.
	 */
	void resetSeed(int seed);

	int getSeed();

private:
    /**
     * Private Constructor. Is expected to be run only once.
     */
	Random();
	Random(int seed);
    // Stop the compiler generating methods of copy the object
    Random(Random const& copy);            // Not Implemented
    Random& operator=(Random const& copy); // Not Implemented

    int _seed;
};


#endif /* RANDOM_H_ */
