/*
 * main.cpp
 *
 *  Created on: Nov 16, 2012
 *      Author: cws
 */

#ifdef INSIDE


#include <Rcpp.h>
// #include <RInside.h>	// for the embedded R via RInside
#include <iostream>
#include <map>
#include "rwrapper/wrapper_process_state.h"
#include "rwrapper/wrapper_network.h"
#include "network/MemoryOneModeNetwork.h"
#include "network/OneModeNetwork.h"
#include "network/NetworkUtils.h"
#include "unittest/TestSuite.h"
#include "model/ModelManager.h"
#include "model/TimeModel.h"
#include "model/Updater.h"
#include "model/MultinomialChoiceNetworkChangeModel.h"
#include "model/saom/SaomEffect.h"
#include "processstate/ProcessState.h"
#include "simulator/Simulator.h"

using namespace Rcpp;

int main(int argc, char *argv[]){

	/*
	// define the process state
	ProcessState myProcessState;
	int nActors = 100;

	MemoryOneModeNetwork network(nActors, true, false);
	network.addTie(1,2);

	size_t indexNetwork = myProcessState.addNetwork(&network);

	// provokes uncaught exception
	// NetworkUtils::dumpNetwork((OneModeNetwork *) (myProcessState.getNetwork(1)),2);

	// define the models and the ModelManager

	ModelManager myModelManager;

	std::set<std::pair<SaomEffect*, double> *> effectParameterPairs;

	effectParameterPairs.insert(new std::pair<SaomEffect*, double>(new DensityEffect(indexNetwork), -3.0));
	effectParameterPairs.insert(new std::pair<SaomEffect*, double>(new ReciprocityEffect(indexNetwork), 1.0));

	// one model chain per actor
	for (int i = 0; i < nActors; i++){

		TieSwapUpdater * tieSwapUpdater = new TieSwapUpdater(indexNetwork);
		MultinomialChoiceNetworkChangeModel* saom =
				new MultinomialChoiceNetworkChangeModel(
						i,
						indexNetwork,
						&effectParameterPairs,
						new std::vector<Updater*>(1, tieSwapUpdater));
		PoissonTimeModel* poissonModel = new PoissonTimeModel(2.0);
		myModelManager.addTimeModel(poissonModel);
		myModelManager.addChangeModel(poissonModel, saom);
		myModelManager.addUpdater(saom, tieSwapUpdater);
	}


//	ExponentialDecayTimeUpdater* timeUpdater = new ExponentialDecayTimeUpdater(indexNetwork, halfLife, resetValue);
//	myModelManager.addTimeUpdater(timeUpdater);

	// define Simulator and simulate

	double periodLength = 1;
	Simulator mySimulator(&myProcessState, &myModelManager, periodLength);

	mySimulator.simulate();

	NetworkUtils utils;
	utils.dumpNetwork((OneModeNetwork*)myProcessState.getNetwork(indexNetwork),1);

	*/

	int nActors = 1;

	MemoryOneModeNetwork network(nActors, true, false);

	ProcessState myProcessState;
	size_t indexNetwork = myProcessState.addNetwork(&network);

	ModelManager myModelManager;
	PoissonTimeModel * poissonModel = new PoissonTimeModel(100);

	MultinomialChoiceNetworkChangeModel * saom = new MultinomialChoiceNetworkChangeModel(
			0,
			indexNetwork,
			new std::set<std::pair<SaomEffect*, double> *>(),
			new std::vector<Updater*>(1, new TieSwapUpdater(indexNetwork)));

	// saom->setDebugMode();

	myModelManager.addTimeModel(poissonModel);
	myModelManager.addChangeModel(poissonModel, saom);

	double periodLength = 10;
	Simulator mySimulator(&myProcessState, &myModelManager, periodLength);



	mySimulator.setVerbose();
	mySimulator.setDebug(false);
	mySimulator.simulate();

	// mySimulator.setVerbose(true);
	// mySimulator.simulateOLD(); // deprecated simulation

	// just doing the right number of changes
	// without model manager and simulatorinvolved
	int nIterations = 1000;
	clock_t timeStart2 = clock();
	for (int i = 0; i< nIterations; i++){
		poissonModel->getTimeSpan(&myProcessState);
		ModelResult * result = saom->getChange(&myProcessState);
		delete result;
	}

	double duration2 = (double)(clock() - timeStart2)/CLOCKS_PER_SEC;

	std::cout << "Same without simulation" << std::endl;
	printf("Simulation time: %.2fs\n", duration2);
	printf("Time per iteration: %.7fs\n", duration2 / ((double) nIterations));

	delete poissonModel;

	// Run unit tests
	runUnitTestSuite();

	return 0;
}


#endif
