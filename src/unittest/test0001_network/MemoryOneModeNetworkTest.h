/*
 * MemoryOneModeNetworkTest.h
 *
 *  Created on: Nov 21, 2012
 *      Author: cws
 */

#ifndef MEMORYONEMODENETWORKTEST_H_
#define MEMORYONEMODENETWORKTEST_H_

#include <vector>
#include "NetworkUtils.h"
#include "ValuedMemoryOneModeNetwork.h"
#include "MemoryOneModeNetwork.h"
#include "../processstate/ProcessState.h"

/*
 * Generates a valued network in which the first node (0) has a
 * tie TO all other nodes and the second node (1) a tie FROM each
 * node. No reflexive ties are generated.
 * 0 -> ALL -> 1
 */
MemoryOneModeNetwork * getTwoStarNetwork(int size){
	// at least two nodes are necessary
	if (size<2) return NULL;

	// set ties from actor 1 and to actor 2 to value 1.0
	std::vector<std::vector<double> > graph(size, std::vector<double>(size));
	for (int i = 0; i < size; i++){
		for (int j = 0; j <size; j++){
			if (i == 0) // actor 1
				graph[i][j] = 1.0;
			if (j == 1) // actor 2
				graph[i][j] = 1.0;
			if (i == j) // no reflexive ties
				graph[i][j] = 0.0;
		}
	}

	MemoryOneModeNetwork *net2 = new MemoryOneModeNetwork(graph);
	return net2;

}

void networkConstructorTest() {
	// using the constructor (int) defining the size
	int size = 800;
	MemoryOneModeNetwork *net = new MemoryOneModeNetwork(size);
	ASSERTM("Network has wrong size", net->getSize() == size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			ASSERT(!net->hasTie(i,j));
	delete net;

	// using the constructor (2d vector)
	size = 10;
	std::vector<std::vector<double> > graph(size, std::vector<double>(size));
	graph[1][2] = 1.0;
	ASSERT(graph[1][2] == 1.0);
	ASSERT(graph[1][3] == 0.0);
	graph[3][5] = 1.0;
	graph[3][6] = 0.0;
	MemoryOneModeNetwork *net2 = new MemoryOneModeNetwork(graph);
	ASSERT(net2->getSize() == size);
	ASSERT(net2->hasTie(1,2));
	ASSERT(net2->hasTie(3,5));
	ASSERT(!net2->hasTie(3,6));
	ASSERT(!net2->hasTie(3,7));
	delete net2;

}

void setAddHasRemoveTieTest() {
	int size = 10;
	MemoryOneModeNetwork *net = new MemoryOneModeNetwork(size);
	net->setTie(1,2,1.0);
	ASSERT(net->hasTie(1,2));
	ASSERT(!net->hasTie(1,1));
	net->addTie(1,3);
	ASSERT(net->hasTie(1,3));
	net->setTie(1,4,0.0);
	ASSERT(!net->hasTie(1,4));
	net->addTie(2,5);
	net->setTie(1,2,1.0);
	ASSERT(net->hasTie(1,2));
	net->removeTie(1,2);
	ASSERT(!net->hasTie(1,2));
	delete net;
}

void degreeMapTest(){
	int size = 20;
	MemoryOneModeNetwork* net = getTwoStarNetwork(size);

	// NetworkUtils utils;
	// utils.dumpNetwork(net);
	// std::cout << "Two degree values of 0: " << net->getOutDegree(0)
	//		<< " " << net->getInDegree(0);
	ASSERT_EQUAL((size - 1), (net->getOutDegree(0)));
	ASSERT((net->getInDegree(1)) == (size - 1));
	ASSERT((net->getInDegree(2)) == 1);
	ASSERT((net->getOutDegree(2)) == 1);

	net->addTie(0,2); // changes nothing
	ASSERT((net->getOutDegree(0)) == (size - 1));
	net->removeTie(0,2);
	// utils.dumpNetwork(net);
	// std::cout << "Two degree values of 0: " << net->getOutDegree(0)
	// 	<< " " << net->getInDegree(0);
	ASSERT((net->getOutDegree(0)) == (size - 2));
	ASSERT((net->getInDegree(2)) == 0);
	net->addTie(2,2);
	ASSERT((net->getInDegree(2)) == 0);
	ASSERT((net->getOutDegree(2)) == 1);
	net->addTie(2,3);
	ASSERT((net->getInDegree(3)) == 2);
	ASSERT((net->getOutDegree(2)) == 2);
}

void neighborSetTest(){
	int size = 20;
	MemoryOneModeNetwork* net = getTwoStarNetwork(size);
	std::set<int> n0 = net->getOutgoingNeighbors(0);
	ASSERT( (n0.size()) == (size -1) );
	ASSERT( n0.find(1) != n0.end() ); // 1 is a neighbor of 0
	ASSERT( n0.find(0) == n0.end() ); // 0 is not a neighbor of 0
	std::set<int> n1 = net->getOutgoingNeighbors(1);
	ASSERT( (n1.size()) == 0 );
	n1.insert(100);
	n1 = net->getOutgoingNeighbors(1);
	ASSERT( (n1.size()) == 0 );

	net->addTie(1,0);
	net->removeTie(0,1);
	n0 = net->getOutgoingNeighbors(0);
	n1 = net->getOutgoingNeighbors(1);
	ASSERT( n1.find(0) != n0.end() );
	ASSERT( n0.find(1) == n0.end() );

	delete net;
}

void neighborSetTestWithMultipleChanges(){
	int nActors = 4;

	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(nActors);

	network->addTie(0,1);

	ProcessState * processState = new ProcessState();
	int networkIndex = processState->addNetwork(network);

	// iterate over all actors
	for (int j = 0; j < nActors; j++){

		int i = 0;

		if (processState->getNetwork(networkIndex)->hasTie(i, j)){
			processState->getNetwork(networkIndex)->removeTie(i, j);
			std::cout << "removing tie " << i << "->" << j << std::endl;
		}
		else {
			processState->getNetwork(networkIndex)->addTie(i, j);
			std::cout << "adding tie " << i << "->" << j << std::endl;
		}


	} // iterate over all actors

	// now expecting a network with ties
	// 0->2
	// 0->3

	MemoryOneModeNetwork * network2 =
			dynamic_cast<MemoryOneModeNetwork*>(
					processState->getNetwork(networkIndex));

	NetworkUtils::dumpNetwork(network2);
	NetworkUtils::dumpInternalObjects(network2);

	ASSERT_EQUAL(2, network2->getOutgoingNeighbors(0).size());
	ASSERT_EQUAL(2, network2->getOutDegree(0));
	std::set<int> out0 = network2->getOutgoingNeighbors(0);
	ASSERT(out0.find(1) == out0.end());
	ASSERT(out0.find(2) != out0.end());
	ASSERT(out0.find(3) != out0.end());

	ASSERT_EQUAL(1, network2->getIncomingNeighbors(2).size());
	ASSERT_EQUAL(1, network2->getInDegree(2));
	std::set<int> in2 = network2->getIncomingNeighbors(2);
	std::set<int> in3 = network2->getIncomingNeighbors(3);

	ASSERT(in2.find(0) != in2.end());
	ASSERT(in3.find(0) != in3.end());
	ASSERT(in2.find(1) == in2.end());

}

void overlappingNeighborsTest(){
	int size = 20;

	MemoryOneModeNetwork* net = getTwoStarNetwork(size);

	std::set<int> nodeSet1 = net->getIntermediateNodesForward(0,1);

	ASSERT(nodeSet1.size() == (size - 2) );
	ASSERT(nodeSet1.find(2) != nodeSet1.end()); // 2 is in the set
	ASSERT(nodeSet1.find(1) == nodeSet1.end()); // 1 is not

	net->addTie(2,3);
	std::set<int> nodeSet2 = net->getIntermediateNodesBackward(1,0);
	std::set<int> nodeSet3 = net->getIntermediateNodesUp(0,2);
	std::set<int> nodeSet4 = net->getIntermediateNodesDown(1,2);
	std::set<int> nodeSet5 = net->getIntermediateNodesDown(2,1);
	ASSERT(nodeSet2.size() == (size - 2));
	ASSERT(nodeSet3.size() == 2 ); // node 1 and 3
	// std::cout << "Size: " << nodeSet4.size() << std::endl;
	// for (std::set<int>::iterator it = nodeSet4.begin(); it != nodeSet4.end(); it++)
	// 	std::cout << *it << " ";
	ASSERT(nodeSet4.size() == 1 ); // node 0
	ASSERT(nodeSet5 == nodeSet4 ); // is such a comparison valid?

	delete net;

}

void unspecificValuedTieUpdateTest(){
	int size = 5;
	ValuedMemoryOneModeNetwork* net = new ValuedMemoryOneModeNetwork(size);
	net->addTie(0, 1);

	// NetworkUtils utils;
	// utils.dumpNetwork(net);

	ASSERT(net->getTieValue(0,1) == 1.0);

	net->multiplyTieValues(1.1);
	ASSERT(net->getTieValue(0,1) == 1.1);

	net->addToTieValues(1.0);
	ASSERT(net->getTieValue(0,1) == 2.2);

	net->multiplyTieValues(exp(1));
	ASSERT(net->getTieValue(0,1) == ( 2.2 * exp(1) ) );

	delete net;
}

void thresholdValueTest(){
	int size = 5;
	ValuedMemoryOneModeNetwork * net = new ValuedMemoryOneModeNetwork(size);
	net->addTie(0, 1);
	ASSERT_EQUAL(1, net->getTieValue(0,1));
	ASSERT_EQUAL(1, net->getMinTieValueAbove(0.5));
	ASSERT(net->getMaxTieValueBelow(0.5) == 0.0);
	net->setTie(0,1,1.2);
	net->setTie(1,0,0.8);
	net->setTie(0,2,1.3);
	net->setTie(1,2,0.85);
	ASSERT(net->getMinTieValueAbove(1.0) == 1.2);
	ASSERT(net->getMinTieValueAbove(0.95) == 1.0);
	ASSERT(net->getMinTieValueAbove(0.85) == 1.0);
	ASSERT(net->getMinTieValueAbove(0.8) == 0.85);
	ASSERT(net->getMinTieValueAbove(1.3) == 1.3);
	ASSERT(net->getMaxTieValueBelow(1.0) == 0.85);
	ASSERT(net->getMaxTieValueBelow(0.8) == 0.0);
	ASSERT(net->getMaxTieValueBelow(1.3) == 1.2);
	ASSERT(net->getMaxTieValueBelow(0.999) == 0.85);

	delete net;
}

void testNetworkUtilsCountTiesAndDensity(){
	MemoryOneModeNetwork * network = getTwoStarNetwork(3);

	NetworkUtils::dumpNetwork(network);

	ASSERT_EQUAL(3,NetworkUtils::getNumberOfTies(network));
	ASSERT_EQUAL(0,NetworkUtils::getNumberOfReciprocalTies(network));
	ASSERT_EQUAL(0.5, NetworkUtils::getDensity(network));
	network->addTie(1,0);
	ASSERT_EQUAL(4,NetworkUtils::getNumberOfTies(network));
	ASSERT_EQUAL(2,NetworkUtils::getNumberOfReciprocalTies(network));
	ASSERT_EQUAL(4.0/6.0, NetworkUtils::getDensity(network));
	network->addTie(2,0);
	ASSERT_EQUAL(5,NetworkUtils::getNumberOfTies(network));
	ASSERT_EQUAL(4,NetworkUtils::getNumberOfReciprocalTies(network));
	ASSERT_EQUAL(5.0/6.0, NetworkUtils::getDensity(network));

	// adding and deleting actors
	network->deleteActor(0);
	network->addActor(0); // now has nor more ties
	ASSERT_EQUAL(1,NetworkUtils::getNumberOfTies(network));
	ASSERT_EQUAL(0,NetworkUtils::getNumberOfReciprocalTies(network));
	ASSERT_EQUAL(1.0/6.0, NetworkUtils::getDensity(network));

	network->deleteActor(0); // death
	network->addActor(3); // birth
	network->addTie(3,1);
	network->addTie(3,2);
	ASSERT_EQUAL(3,NetworkUtils::getNumberOfTies(network));
	ASSERT_EQUAL(0,NetworkUtils::getNumberOfReciprocalTies(network));
	ASSERT_EQUAL(3.0/6.0, NetworkUtils::getDensity(network));


	delete network;
}

void testNetworkUtilsHammingDistance(){
	MemoryOneModeNetwork * network1 = getTwoStarNetwork(10);
	MemoryOneModeNetwork * network2 = getTwoStarNetwork(10);
	MemoryOneModeNetwork * network3 = new MemoryOneModeNetwork(10);
	std::vector<double> onLineOfGraph(10, 1.0);
	std::vector<std::vector<double> > graph(10, onLineOfGraph);
	MemoryOneModeNetwork * network4 = new MemoryOneModeNetwork(graph);

	ASSERT_EQUAL(0, NetworkUtils::getHammingDistance(network1, network2));
	ASSERT_EQUAL(9+8, NetworkUtils::getHammingDistance(network1, network3));
	ASSERT_EQUAL(10*9, NetworkUtils::getHammingDistance(network3, network4));

	delete network1;
	delete network2;
	delete network3;
	delete network4;
}

void testNetworkUtilsGetNRandomNodes(){

	int nIterations = 100000;
	int networkSize = 5;
	int nNodesToSelect = 3;
	double allowedDeviation = 0.01;

	std::vector<double> ratios(networkSize, 0);
	std::vector<double> expectations(networkSize, 1.0 / (double) networkSize);


	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(networkSize);
	NetworkUtils utils;

	for (int i = 0; i < nIterations; i++){

		std::set<int> nodes = utils.getNRandomNodes(network, nNodesToSelect);
		ASSERT_EQUAL(nodes.size(), nNodesToSelect);

		// should not be possible in a set
		ASSERT(*(++nodes.begin()) != *(++(++nodes.begin())));

		std::set<int>::iterator itNodes = nodes.begin();

		for (; itNodes != nodes.end(); ++itNodes){
			ratios[*itNodes] += 1.0 / ((double)nNodesToSelect * (double)nIterations);
		}

	}

	for (int i = 0; i < ratios.size(); i++){
		std::cout << "Ratio of node " << i << " selections: " << ratios[i] <<
				" (" << expectations[i] << " +- " <<
				fabs(expectations[i] - ratios[i]) << ")"<< std::endl;
		ASSERT(fabs(expectations[i] - ratios[i]) < allowedDeviation);
	}



	delete network;
}

void deathAndBirthOfNodesTest(){
	// TODO implement
}

void regularLatticeNetworkUtilsTest(){
	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(10);

	NetworkUtils::addRingLatticeTiesToNetwork(network, 4);

	NetworkUtils::dumpNetwork(network);

	ASSERT(network->hasTie(0,1));
	ASSERT(network->hasTie(0,2));
	ASSERT(network->hasTie(1,0));
	ASSERT(network->hasTie(2,0));
	ASSERT(network->hasTie(0,9));
	ASSERT(network->hasTie(0,8));
	ASSERT(network->hasTie(9,0));
	ASSERT(network->hasTie(8,0));
	ASSERT(network->hasTie(5,4));
	ASSERT(network->hasTie(5,3));
	ASSERT(network->hasTie(3,5));
	ASSERT(network->hasTie(9,8));
	ASSERT(network->hasTie(9,7));

	ASSERT(!network->hasTie(5,2));
	ASSERT(!network->hasTie(0,3));
	ASSERT(!network->hasTie(0,4));
	ASSERT(!network->hasTie(3,0));
	ASSERT(!network->hasTie(4,0));
	ASSERT(!network->hasTie(9,6));
	ASSERT(!network->hasTie(6,9));
}

cute::suite getTestSuiteMemoryOneModeNetwork(){
	cute::suite s;

	s.push_back(CUTE(networkConstructorTest));
	s.push_back(CUTE(setAddHasRemoveTieTest)); // add tests for undirected networks
	s.push_back(CUTE(degreeMapTest));
	s.push_back(CUTE(neighborSetTest));
	s.push_back(CUTE(overlappingNeighborsTest));
	s.push_back(CUTE(neighborSetTestWithMultipleChanges));
	// DO not work with binary MemoryOneModeNetwork implementation
	s.push_back(CUTE(unspecificValuedTieUpdateTest));
	// s.push_back(CUTE(thresholdValueTest));
	s.push_back(CUTE(testNetworkUtilsCountTiesAndDensity));
	s.push_back(CUTE(testNetworkUtilsHammingDistance));
	s.push_back(CUTE(testNetworkUtilsGetNRandomNodes));
	s.push_back(CUTE(regularLatticeNetworkUtilsTest));

	return s;
}



#endif /* MEMORYONEMODENETWORKTEST_H_ */
