/*
 * WattsStrogatzTest.h
 *
 *  Created on: Feb 27, 2013
 *      Author: cws
 */

#ifndef WATTSSTROGATZTEST_H_
#define WATTSSTROGATZTEST_H_

#include "cute.h"
#include "WattsStrogatzRandomRewiringChangeModel.h"
#include <stddef.h>

void wattsStrogatzDensityTest(){

	int nSimulations = 10000;
	// each of the six ties is equally likely in the long run
	std::vector<double> expectedRatioOfTies(12, 1.0/12.0);
	std::vector<double> averageRatioOfTies(12, 0);
	double allowedDeviation = 0.01;

	bool debugTest = false;

	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(4);
	network->addTie(0,1);
	network->addTie(1,2);
	network->addTie(2,3);
	network->addTie(3,0);
	ProcessState * processState = new ProcessState;
	size_t networkIndex = processState->addNetwork(network);

	WattsStrogatzRandomRewiringChangeModel wsModel(networkIndex);
	wsModel.setDebug(debugTest);

	int i = -1;
	int j = -1;
	for (int iSim = 0; iSim < nSimulations; iSim++){

		TwoTiesModelResult * result = dynamic_cast<TwoTiesModelResult*>(
				wsModel.getChange(processState));

		network->removeTie(result->getActorIndex1(), result->getActorIndex2());
		if (debugTest) std::cout << "Deleting tie " << result->getActorIndex1() <<
				"->" << result->getActorIndex2() << std::endl;

		network->addTie(result->getActorIndex3(), result->getActorIndex4());
		if (debugTest) std::cout << "Adding tie " << result->getActorIndex3() <<
				"->" << result->getActorIndex4() << std::endl;

		// change a tie from the same actor twice in a row
		if (i == result->getActorIndex1()){
			if (debugTest) std::cout << "i, j: " << i << "," << j << std::endl;
			if (debugTest) std::cout << "actorIndex1, actorIndex2: " << result->getActorIndex1() <<
					"," << result->getActorIndex2() << std::endl;
			ASSERT_EQUAL(j, result->getActorIndex2());
		}


		i = result->getActorIndex3();
		j = result->getActorIndex4();


		delete result;

		int index = 0;
		if (j > i) index = 3 * i + j - 1;
		if (j < i) index = 3 * i + j;
		if (debugTest) std::cout << i << "->" << j << " (index: " << index << ")" << std::endl;

		averageRatioOfTies[index] += 1 / (double) nSimulations;
	}


	// output results
	for (int k = 0; k < 12; k++){
		std::cout << "Ratio of tie #" << k << ": " << averageRatioOfTies[k] <<
				" ("<< expectedRatioOfTies[k] << ")" << std::endl;
	}

	// testing results
	for (int k = 0; k < 12; k++){
		ASSERT(fabs(expectedRatioOfTies[k] - averageRatioOfTies[k]) < allowedDeviation);
	}



}


cute::suite getWattsStrogatzTests(){
	cute::suite s;

	s.push_back(CUTE(wattsStrogatzDensityTest));


	return s;
}


#endif /* WATTSSTROGATZTEST_H_ */
