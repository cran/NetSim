/*
 * UtilsTest.h
 *
 *  Created on: Aug 26, 2013
 *      Author: cws
 */

#ifndef UTILSTEST_H_
#define UTILSTEST_H_

#include "cute.h"
#include "../utils/Random.h"


void setRandomSeedTest(){

	int seed = Random::getInstance().getSeed();
	std::cout << "Seed: " << seed << "\n";


	Random::getInstance().resetSeed(100);
    double randA = Random::getInstance().getRandom();
	double randB = Random::getInstance().getRandom();
	Random::getInstance().resetSeed(100);
	double randC = Random::getInstance().getRandom();
	double randD = Random::getInstance(2).getRandom();
	Random::getInstance().resetSeed(100);
	double randE = Random::getInstance().getRandom();

	std::cout << "Seed: " << Random::getInstance().getSeed() << "\n";

	std::cout << randA << "\n"
			<< randB << "\n"
			<< randC << "\n"
			<< randD << "\n";

	ASSERT(randB == randD);
	ASSERT(randA == randC);
	ASSERT(randA != randB);
	ASSERT(randA == randE);

}

void getSeedTest(){
	Random::getInstance().resetSeed(1);
	int seed = Random::getInstance().getSeed();

	ASSERT (seed == 1);

}

cute::suite getUtilsTests(){
	cute::suite s;

	s.push_back(CUTE(setRandomSeedTest));
	s.push_back(CUTE(getSeedTest));


	return s;
}



#endif /* UTILSTEST_H_ */
