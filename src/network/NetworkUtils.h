/*
 * NetworkUtils.h
 *
 *  Created on: Nov 21, 2012
 *      Author: cws
 */

#ifndef NETWORKUTILS_H_
#define NETWORKUTILS_H_

#include "OneModeNetwork.h"
#include "MemoryOneModeNetwork.h"
#include "../utils/Random.h"
#include <iostream>
#include <cstdio>
#include <set>
#include <utility>
#include <stdexcept>
#include "../io/Output.h"
#include <math.h>

class NetworkUtils{

public:
	/**
	 * for debugging
	 */
	static void dumpInternalObjects(MemoryOneModeNetwork* network);

	static void dumpNetwork(MemoryOneModeNetwork* network, int round = 2);

	static double getDensity(MemoryOneModeNetwork* network);

	static int getNumberOfTies(MemoryOneModeNetwork* network);

	static int getNumberOfReciprocalTies(MemoryOneModeNetwork* network);

	static int getHammingDistance(MemoryOneModeNetwork* network1, MemoryOneModeNetwork * network2);

	static std::set<int> getNRandomNodes(Network * network, int n);

	/**
	 * returns a random tie of network as an integer pair.
	 * If no tie can be chosen it returns a (-1, -1) pair
	 */
	static std::pair<int, int> getRandomTie(MemoryOneModeNetwork * network);

	/**
	 * add ties with a probability p to an existing network.
	 * Currently existing ties are ignored
	 */
	static void addRandomTiesToNetwork(Network * network, double p);

	static void addRingLatticeTiesToNetwork(MemoryOneModeNetwork * network, int nReciprocalEdges = 4);

private:
	static int countTies(MemoryOneModeNetwork* network, bool reciprocal);

};


#endif /* NETWORKUTILS_H_ */
