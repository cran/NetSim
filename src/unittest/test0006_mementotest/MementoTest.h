/*
 * MementoTest.h
 *
 *  Created on: Feb 6, 2013
 *      Author: cws
 */

#ifndef MEMENTOTEST_H_
#define MEMENTOTEST_H_

#include "cute.h"
#include "MemoryOneModeNetwork.h"
#include "ProcessState.h"
#include "NetworkUtils.h"

void networkMementoTest(){
	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(5);

	network->addTie(0,1);
	network->addTie(0,2);
	network->addTie(0,3);
	MemoryOneModeNetworkMemento * memento = network->saveToMemento();
	network->removeTie(0,1);
	network->addTie(0,4);
	network->restoreFromMemento(memento);
	ASSERT(network->hasTie(0,1));
	ASSERT(network->hasTie(0,2));
	ASSERT(network->hasTie(0,3));
	ASSERT(!network->hasTie(0,4));


	delete network;
}

void processStateMementoTestWithNetwork(){
	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(5);

	ProcessState * processState = new ProcessState();

	size_t index = processState->addNetwork(network);

	network->addTie(0,1);
	network->addTie(0,2);
	network->addTie(0,3);
	ProcessStateMemento * memento = processState->saveToMemento();
	network->removeTie(0,1);
	network->addTie(0,4);
	processState->restoreFromMemento(memento);
	ASSERT(network->hasTie(0,1));
	ASSERT(network->hasTie(0,2));
	ASSERT(network->hasTie(0,3));
	ASSERT(!network->hasTie(0,4));
	ASSERT(processState->getNetwork(index)->hasTie(0,1));
	ASSERT(processState->getNetwork(index)->hasTie(0,2));
	ASSERT(processState->getNetwork(index)->hasTie(0,3));
	ASSERT(!processState->getNetwork(index)->hasTie(0,4));

	NetworkUtils utils;
	MemoryOneModeNetwork * net1 = dynamic_cast<MemoryOneModeNetwork*>(processState->getNetwork(index));
	utils.dumpNetwork(net1);

	delete network;
	delete processState;
}

void multipleMementoSetAndResetTest(){

	int nActors = 5;


	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(nActors);

	network->addTie(0,1);

	ProcessState * processState = new ProcessState();
	int networkIndex = processState->addNetwork(network);

	std::vector<Updater*> updaters(1, new TieSwapUpdater(networkIndex));

	MemoryOneModeNetworkMemento * networkMemento = network->saveToMemento();

	ProcessStateMemento * processMemento = processState->saveToMemento();

	// iterate over all actors
	for (int j = 1; j < nActors; j++){

		int i = 0;

		// update of tie consisting of exactly one tie swap
		for (std::vector<Updater*>::iterator it =
				updaters.begin(); it != updaters.end(); ++it){

			TieModelResult * result = new TieModelResult(i,j);
			(*it)->update(processState, result);
			delete result;
		}

		// network->addTie(0,4);

		MemoryOneModeNetwork * net1 =
				dynamic_cast<MemoryOneModeNetwork * >(processState->getNetwork(networkIndex));
		//if (j == (nActors - 1))
		//	NetworkUtils::dumpNetwork(net1);
		//if (j == (nActors - 1))
		//	NetworkUtils::dumpInternalObjects(net1);


		// reset process state
		processState->restoreFromMemento(processMemento);

	} // iterate over all actors

	// check restore from process state first

	MemoryOneModeNetwork * net2 =
			dynamic_cast<MemoryOneModeNetwork * >(processState->getNetwork(networkIndex));
	NetworkUtils::dumpNetwork(net2);
	//NetworkUtils::dumpInternalObjects(net2);

	// network from process state memento
	ASSERT(processState->getNetwork(networkIndex)->hasTie(0,1));
	ASSERT(!processState->getNetwork(networkIndex)->hasTie(0,2));
	ASSERT(!processState->getNetwork(networkIndex)->hasTie(1,0));

	delete processMemento;

	// RESTORE network directly

	network->restoreFromMemento(networkMemento);
	ASSERT(processState->getNetwork(networkIndex)->hasTie(0,1));
	ASSERT(!processState->getNetwork(networkIndex)->hasTie(0,2));
	ASSERT(!processState->getNetwork(networkIndex)->hasTie(1,0));

}


cute::suite getMementoTests(){
	cute::suite s;

	s.push_back(CUTE(networkMementoTest));
	s.push_back(CUTE(processStateMementoTestWithNetwork));
	s.push_back(CUTE(multipleMementoSetAndResetTest));

	return s;
}

#endif /* MEMENTOTEST_H_ */
