/*
 * SaomTest.h
 *
 *  Created on: Nov 26, 2012
 *      Author: cws
 */

#ifndef SAOMTEST_H_
#define SAOMTEST_H_

#include <vector>
#include <utility>
#include <set>
#include "../../model/saom/SaomEffect.h"
#include "../../model/saom/EffectContainerManager.h"
#include "../../model/ModelManager.h"
#include "../../model/MultinomialChoiceNetworkChangeModel.h"
#include "../../model/TimeModel.h"
#include "../../processstate/ProcessState.h"
#include "../../simulator/Simulator.h"




void poissonparameterTest(){

	// int nActors = 100;
	double rate = 1.0;
	double periodLength = 1000;
	int nSimulations = 100;
	double deviationOfAverage = 5; // What is the 99% confidence interval?

	double averageSteps = 0;

	for (int iSim = 0; iSim< nSimulations; iSim++){
		ModelManager modelmanager;
		PoissonTimeModel * poissonModel = new PoissonTimeModel(rate);
		modelmanager.addTimeModel(poissonModel);

		ProcessState processState;
		// processState.addNetwork(new MemoryOneModeNetwork(nActors));

		Simulator simulator(&processState, &modelmanager, periodLength);
		simulator.setVerbose(false);

		simulator.simulate();

		averageSteps += ( (double) simulator.getIterationSteps() ) / (double) nSimulations;

		delete poissonModel;
	}

	std::cout << "Average number of iteration steps: " << averageSteps << std::endl;

	ASSERT(fabs(averageSteps - (double) (rate * periodLength)) < deviationOfAverage);

}




/**
 * Tests whether a model with a density parameter
 * calculates the correct choice probabilities.
 * The effect itself it tested separately in the @EffectTest suite.
 */
void densityChoiceTest(){

	double densityParameter = -2.0;
	int actorIndex = 0;
	size_t networkIndex = 0;
	int nSimulations = 10000;
	double allowedDeviation = 0.01;

	// calculating expected values
	// network description below
	std::vector<double> expectedValues(5,0.0);
	std::vector<double> estimatedValues(5,0.0);
	double denominator = exp(densityParameter * 3)
			+ 3 * exp( densityParameter * 2)
			+ exp( densityParameter * 4);

	std::cout << "denominator: " << denominator << std::endl;

	// probability to... keep state
	expectedValues[0] = exp(densityParameter * 3) / denominator;
	// ... remove a tie
	expectedValues[1] = exp(densityParameter * 2) / denominator;
	expectedValues[2] = exp(densityParameter * 2) / denominator;
	expectedValues[3] = exp(densityParameter * 2) / denominator;
	// ... add a tie
	expectedValues[4] = exp(densityParameter * 4) / denominator;


	std::set<std::pair<SaomEffect*, double> *> effects;
	effects.insert(new std::pair<SaomEffect*, double>(
			new DensityEffect(0), densityParameter));

	MultinomialChoiceNetworkChangeModel saom(
			actorIndex,
			networkIndex,
			&effects,
			new std::vector<Updater*>(1,new TieSwapUpdater(networkIndex)));

	saom.setDebugMode(false);

	for (int iSim = 0; iSim < nSimulations; iSim++){

		MemoryOneModeNetwork network(5);
		network.addTie(0,1);
		network.addTie(0,2);
		network.addTie(0,3);

		ProcessState processState;
		processState.addNetwork(&network);

		// this is what is actually tested
		// if (iSim == 0) saom.setDebugMode(true); // test verbose
		ModelResult * result = saom.getChange(&processState);

		TieModelResult * actorResult = dynamic_cast<TieModelResult*>(result);

		estimatedValues[actorResult->getActorIndex2()] += 1 / ((double) nSimulations);

	}

	std::cout << "i: " << "expected" << " / " << "estimated" << std:: endl;
	for (int i = 0; i < 5; i++){
		std::cout << i << ": " << expectedValues[i] << " / " << estimatedValues[i] << std:: endl;
	}

	ASSERT(fabs(expectedValues[0] - estimatedValues[0]) < allowedDeviation);
	ASSERT(fabs(expectedValues[1] - estimatedValues[1]) < allowedDeviation);
	ASSERT(fabs(expectedValues[2] - estimatedValues[2]) < allowedDeviation);
	ASSERT(fabs(expectedValues[3] - estimatedValues[3]) < allowedDeviation);
	ASSERT(fabs(expectedValues[4] - estimatedValues[4]) < allowedDeviation);

}

void transitivityChoiceTest(){

	int networkIndex = 0;
	int actorIndex = 0; // focal actor
	int nSimulations = 10000;
	double parameter = 1.0;
	std::vector<double> estimatedValues(5,0.0);
	std::vector<double> expectedValues(5,0.0);
	double allowedDeviation = 0.01;


	MemoryOneModeNetwork network(5);
	network.addTie(0,1);
	network.addTie(1,0);
	network.addTie(0,2);
	network.addTie(0,3);
	network.addTie(2,1);
	network.addTie(2,4);
	network.addTie(3,4);
	network.addTie(4,3);
	// has on transitive triad 0, 1, 2

	double denominator =
			exp(parameter * 1) + // doing nothing
			exp(parameter * 0) + // removing tie to 1
			exp(parameter * 0) + // removing tie to 2
			exp(parameter * 1) + // removing tie to 3
			exp(parameter * 4);  // adding tie to 4
	expectedValues[0] = exp(parameter * 1) / denominator;
	expectedValues[1] = exp(parameter * 0) / denominator;
	expectedValues[2] = exp(parameter * 0) / denominator;
	expectedValues[3] = exp(parameter * 1) / denominator;
	expectedValues[4] = exp(parameter * 4) / denominator;


	std::set<std::pair<SaomEffect*, double> *> effects;
	effects.insert(new std::pair<SaomEffect*, double>(
			new TransitivityEffect(networkIndex), parameter));

	MultinomialChoiceNetworkChangeModel saom(
			actorIndex,
			networkIndex,
			&effects,
			new std::vector<Updater*>(1,new TieSwapUpdater(networkIndex)));

	for (int iSim = 0; iSim < nSimulations; iSim++){

		ProcessState processState;
		processState.addNetwork(&network);

		saom.setDebugMode(false);

		ModelResult * result = saom.getChange(&processState);

		TieModelResult * tieResult = dynamic_cast<TieModelResult*>(result);

		estimatedValues[tieResult->getActorIndex2()] += 1 / ((double) nSimulations);

	}

	std::cout << "denominator: " << denominator << std::endl;
	std::cout << "i: " << "expected" << " / " << "estimated" << std:: endl;
	for (int i = 0; i < 5; i++){
		std::cout << i << ": " << expectedValues[i] << " / " << estimatedValues[i] << std:: endl;
	}

	ASSERT(fabs(expectedValues[0] - estimatedValues[0]) < allowedDeviation);
	ASSERT(fabs(expectedValues[1] - estimatedValues[1]) < allowedDeviation);
	ASSERT(fabs(expectedValues[2] - estimatedValues[2]) < allowedDeviation);
	ASSERT(fabs(expectedValues[3] - estimatedValues[3]) < allowedDeviation);
	ASSERT(fabs(expectedValues[4] - estimatedValues[4]) < allowedDeviation);


}

/*
 * E-Mail Christian Steglich, Jan. 28
Jeweils n=100.

Parametersatz 1:
rate period 1 = hoch (=n, =100)
rate period 2 = 8,81
outdegree = -2,18
reciprocity = 2,39

Sollte simulierte Netzwerke liefern, wo
>> Hamming distance zwischen 2. und 3. Netzwerk = 400
>> # ties im 2. und 3. Netzwerk jeweils 400
>> # reciprocal ties im 2. und 3. Netzwerk jeweils 240


Parametersatz 2:
rate period 1 = hoch (=n, =100)
rate period 2 = 22,3
outdegree = -2,24
reciprocity = 2,24

Sollte simulierte Netzwerke liefern, wo
>> Hamming distance zwischen 2. und 3. Netzwerk = 500
>> # ties im 2. und 3. Netzwerk jeweils 300
>> # reciprocal ties im 2. und 3. Netzwerk jeweils 150
 *
 */
void steglichParameterTest(
		double poissonParameter1,
		double poissonParameter2,
		double densityParameter,
		double reciprocityParameter,
		double expectedSingleTie,
		double expectedReciprocity,
		double expectedHammingDistance,
		double allowedDeviation,
		int nSimulations){


	int nActors = 100;
	// TODO set back to Memory...
	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(nActors);
	// initialize with 50% non-reciprocal ties to improve burn in
	for (int iActor = 0; iActor < (nActors - 1); iActor++){
		for (int jActor = (iActor+1); jActor < nActors; jActor++){
			network->addTie(iActor, iActor);
		}
	}

	ProcessState * processState = new ProcessState();
	int networkIndex = processState->addNetwork(network);

	ModelManager * modelManager = new ModelManager();

	// specify SAOM
	EffectContainerManager * effectManager = new EffectContainerManager();
	effectManager->addToEffectContainer(
			new DensityEffect(networkIndex),
			densityParameter);
	effectManager->addToEffectContainer(
			new ReciprocityEffect(networkIndex),
			reciprocityParameter);


	TieSwapUpdater * tieSwapUpdater = new TieSwapUpdater(networkIndex);

	for (int i = 0; i < nActors; i++){

		PoissonTimeModel * poissonModel = new PoissonTimeModel(
				poissonParameter1);

		MultinomialChoiceNetworkChangeModel * saom =
				new MultinomialChoiceNetworkChangeModel(
						i, networkIndex,
						effectManager->getEffectContainer(),
						new std::vector<Updater*>(1, tieSwapUpdater)
						);

		modelManager->addTimeModel(poissonModel);
		modelManager->addChangeModel(poissonModel, saom);
		modelManager->addUpdater(saom, tieSwapUpdater);
	}

	// burn in
	std::cout << "Burn in simulation:" << std::endl;
	Simulator simulator(processState, modelManager, 1);
	simulator.simulate();

	double periodLength = poissonParameter2/ poissonParameter1;

	double averageTies =
			((double) NetworkUtils::getNumberOfTies(network))
			/ ((double) nSimulations + 1.0);
	double averageReciprocalTies =
			((double) NetworkUtils::getNumberOfReciprocalTies(network))
			/ ((double) nSimulations + 1.0);
	double averageHammingDistance = 0;

	std::cout << "Single ties: " << NetworkUtils::getNumberOfTies(network) << std::endl;
	std::cout << "Reciprocal ties: " << NetworkUtils::getNumberOfReciprocalTies(network) << std::endl;

	// copy network for later comparison
	MemoryOneModeNetwork * oldNetwork = new MemoryOneModeNetwork(network->getGraph());

	std::cout << nSimulations << " shorter simulations:" << std::endl;
	for (int iSim = 0; iSim < nSimulations; iSim++){
		Simulator simulator(processState, modelManager, periodLength);
		simulator.setVerbose(false);
		simulator.simulate();

		// update number of ties and reciprocal ties
		averageTies +=
				((double) NetworkUtils::getNumberOfTies(network))
				/ ((double) nSimulations + 1.0);
		averageReciprocalTies +=
				((double) NetworkUtils::getNumberOfReciprocalTies(network))
				/ ((double) nSimulations + 1.0);

		// calculate Hamming distance and update average distance
		// div nSimulations without +1
		averageHammingDistance +=
			(double) NetworkUtils::getHammingDistance(network, oldNetwork) /
			(double) nSimulations;

		// delete and re-assign old network
		delete oldNetwork;
		oldNetwork = new MemoryOneModeNetwork(network->getGraph());
		std::cout << iSim + 1 << " " << std::flush;
		if ((iSim + 1) % 20 == 0) {
			std::cout << std::endl;
			std::cout << "avg (ties / recip / hamming): " <<
					averageTies * nSimulations / (iSim + 1);
			std::cout << " / " <<
					averageReciprocalTies * nSimulations / (iSim + 1);
			std::cout << " / " <<
					averageHammingDistance * (double) nSimulations / (iSim)<< std::endl;


		}
	}

	delete oldNetwork;

	std::cout << "average ties: " << averageTies << std::endl;
	std::cout << "average reciprocal ties: " << averageReciprocalTies << std::endl;
	std::cout << "average Hamming distance: " << averageHammingDistance << std::endl;

	ASSERT(abs(expectedSingleTie - averageTies) < allowedDeviation);
	ASSERT(abs(expectedReciprocity - averageReciprocalTies) < allowedDeviation);
	ASSERT(abs(expectedHammingDistance - averageHammingDistance) < allowedDeviation);

	delete processState;
	delete modelManager;
	delete effectManager;

}

/**
 * Test 1 proposed by Christian Steglich, 28. Jan. 2013;
 * see description above
 *
 Parametersatz 1:
rate period 1 = hoch (=n, =100)
rate period 2 = 8,81
outdegree = -2,18
reciprocity = 2,39

Sollte simulierte Netzwerke liefern, wo
>> Hamming distance zwischen 2. und 3. Netzwerk = 400
>> # ties im 2. und 3. Netzwerk jeweils 400
>> # reciprocal ties im 2. und 3. Netzwerk jeweils 240
 */
void steglichParameterTest1(){

	double poissonParameter1 = 10; // was 1000
	double poissonParameter2 = 8.81;
	double densityParameter = -2.18;
	double reciprocityParameter = 2.39;
	double expectedSingleTie = 400;
	double expectedReciprocity = 240;
	double expectedHammingDistance = 400;
	double allowedDeviation = 10;
	int nSimulations = 1; // was: 300

	steglichParameterTest(
			poissonParameter1,
			poissonParameter2,
			densityParameter,
			reciprocityParameter,
			expectedSingleTie,
			expectedReciprocity,
			expectedHammingDistance,
			allowedDeviation,
			nSimulations);


}

/**
 * Test 2 proposed by Christian Steglich, 28. Jan. 2013;
 * see description above
 *
Parametersatz 2:
rate period 1 = hoch (=n, =100)
rate period 2 = 22,3
outdegree = -2,24
reciprocity = 2,24

Sollte simulierte Netzwerke liefern, wo
>> Hamming distance zwischen 2. und 3. Netzwerk = 500
>> # ties im 2. und 3. Netzwerk jeweils 300
>> # reciprocal ties im 2. und 3. Netzwerk jeweils 150
 */
void steglichParameterTest2(){

	double poissonParameter1 = 1000;
	double poissonParameter2 = 22.3;
	double densityParameter = -2.24;
	double reciprocityParameter = 2.24;
	double expectedSingleTie = 300;
	double expectedReciprocity = 150;
	double expectedHammingDistance = 500;
	double allowedDeviation = 10;
	int nSimulations = 200; // was 400

	steglichParameterTest(
			poissonParameter1,
			poissonParameter2,
			densityParameter,
			reciprocityParameter,
			expectedSingleTie,
			expectedReciprocity,
			expectedHammingDistance,
			allowedDeviation,
			nSimulations);


}

/**
 * based on working paper on expected network densities
 */
void densityParameterSaomTest(){
	ProcessState processState1;
	ProcessState processState2;
	ModelManager modelManager1;
	ModelManager modelManager2;
	int nActors = 3;
	double densityParameter1 = 0.0;
	double densityParameter2 = -1.0;

	double expectedDensityNull = 0.5;
	double expectedDensityMinusOne = 0.2266792; // from working paper on expected densities.

	double allowedDeviation = 0.02;

	// 100 expected changes per actor to reach stable state
	double rateParameter = 50; // was 100
	double timeSpan = 1;
	int nSimulations = 500; // was 1000

	double averageDensityNull = 0;
	double averageDensityMinusOne = 0;


	size_t networkIndex1 = processState1.addNetwork(new MemoryOneModeNetwork(nActors));
	size_t networkIndex2 = processState2.addNetwork(new MemoryOneModeNetwork(nActors));


	// TODO: if this is moved within the loop, the test crashes.
	std::set<std::pair<SaomEffect*, double> *> effects1;
	effects1.insert(new std::pair<SaomEffect*, double>(
			new DensityEffect(networkIndex1), densityParameter1));
	std::set<std::pair<SaomEffect*, double> *> effects2;
	effects2.insert(new std::pair<SaomEffect*, double>(
			new DensityEffect(networkIndex2), densityParameter2));

	for (int i = 0; i< nActors; i++){
		PoissonTimeModel * poissonModel = new PoissonTimeModel(rateParameter);

		TieSwapUpdater * tieSwapUpdater = new TieSwapUpdater(networkIndex1);

		MultinomialChoiceNetworkChangeModel * saom1 =
				new MultinomialChoiceNetworkChangeModel(
						i,
						networkIndex1,
						&effects1,
						new std::vector<Updater*>(1, tieSwapUpdater));
		MultinomialChoiceNetworkChangeModel * saom2 =
				new MultinomialChoiceNetworkChangeModel(
						i,
						networkIndex2,
						&effects2,
						new std::vector<Updater*>(1, tieSwapUpdater));

		// TODO remove
		// saom1->setDebugMode(true);
		// saom2->setDebugMode(true);

		modelManager1.addTimeModel(poissonModel);
		modelManager1.addChangeModel(poissonModel, saom1);
		modelManager1.addUpdater(saom1, tieSwapUpdater);

		modelManager2.addTimeModel(poissonModel);
		modelManager2.addChangeModel(poissonModel, saom2);
		modelManager2.addUpdater(saom2, tieSwapUpdater);

	}



	for (int iSim = 0; iSim < nSimulations; iSim++){

		Simulator simulator1(&processState1, &modelManager1, timeSpan);
		Simulator simulator2(&processState2, &modelManager2, timeSpan);

		simulator1.setVerbose(false);
		simulator2.setVerbose(false);

		simulator1.simulate();
		simulator2.simulate();

		MemoryOneModeNetwork * network1 = dynamic_cast<MemoryOneModeNetwork*>(processState1.getNetwork(networkIndex1));
		MemoryOneModeNetwork * network2 = dynamic_cast<MemoryOneModeNetwork*>(processState2.getNetwork(networkIndex2));
		NetworkUtils utils;
		double densityNull = utils.getDensity(network1);
		double densityMinusOne = utils.getDensity(network2);
		averageDensityNull += densityNull/nSimulations;
		averageDensityMinusOne += densityMinusOne/nSimulations;

	}

	std::cout << "Average density with parameter 0: " << averageDensityNull << std::endl;
	std::cout << "Average density with parameter -1: " << averageDensityMinusOne << std::endl;

	ASSERT(fabs(averageDensityNull - expectedDensityNull) < allowedDeviation);
	ASSERT(fabs(averageDensityMinusOne - expectedDensityMinusOne) < allowedDeviation);

}

void attributeMultinomialChoiceNetworkChangeModelTest(){

	// Parameters

	int nSimuations = 10000;
	double allowedDeviation = 0.01;

	std::vector<double> poissonParameters(3,0);
	poissonParameters[0] = 1;
	poissonParameters[1] = 2;
	poissonParameters[2] = 3;
	double sumPoisson =
			poissonParameters[0] + poissonParameters[1]+ poissonParameters[2];

	std::vector<double> densityParameters(3,0);
	densityParameters[0] = 1;
	densityParameters[1] = 2;
	densityParameters[2] = -1;

	// Define Process State

	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(3);
	network->addTie(0,1);
	network->addTie(1,2);
	network->addTie(2,0);

	AttributeContainer * poissonParameterContainer =
			new AttributeContainer(poissonParameters);
	AttributeContainer * densityParameterContainer =
			new AttributeContainer(densityParameters);

	ProcessState * processState = new ProcessState();
	size_t networkIndex = processState->addNetwork(network);
	size_t poissonIndex = processState->addAttributeContainer(poissonParameterContainer);
	size_t densityIndex = processState->addAttributeContainer(densityParameterContainer);


	// Calculate expected probabilities

	std::vector<double> denominators(3,0);
	for(int i = 0; i < 3; i++){
		denominators[0] += exp(i *densityParameters[0]);
		denominators[1] += exp(i *densityParameters[1]);
		denominators[2] += exp(i *densityParameters[2]);
	}

	std::vector<std::vector<double> > choiceProbabilities(3, std::vector<double>(3,0));
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if (i == j)
				choiceProbabilities[i][j] = exp(densityParameters[i]) / denominators[i];
			else if (network->hasTie(i, j)) // remove tie
				choiceProbabilities[i][j] = 1 / denominators[i];
			else // add tie
				choiceProbabilities[i][j] = exp(2* densityParameters[i]) / denominators[i];
			std::cout << choiceProbabilities[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::vector<double> activityProbabilities(3,0);
	for(int i = 0; i < 3; i++){
		activityProbabilities[i] = poissonParameters[i] / sumPoisson;
	}

	// test the expected values (test the test)
	for(int i = 0; i < 3; i++){
		ASSERT_EQUAL(1.0 ,choiceProbabilities[i][0] +
				choiceProbabilities[i][1] + choiceProbabilities[i][2]);
	}
	ASSERT_EQUAL(1.0, activityProbabilities[0] +
			activityProbabilities[1] + activityProbabilities[2]);
	ASSERT_EQUAL(1.0 / (1.0 + 2.0 + 3.0), activityProbabilities[0]);

	std::vector<std::vector<double> > tieProbabilities(3, std::vector<double>(3,0));
	double sumProbs = 0;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			tieProbabilities[i][j] =
					activityProbabilities[i] * choiceProbabilities[i][j];
			sumProbs += tieProbabilities[i][j];
		}
	}
	ASSERT_EQUAL(1.0, sumProbs);

	// Simulate Probabilities with Change Model

	std::vector<std::vector<double> > averageTies(3, std::vector<double>(3,0));
	
	AttributeMultinomialChoiceNetworkChangeModel attributeSaom(networkIndex,
			poissonIndex,
			// std::vector<SaomEffect*>(1, new DensityEffect(networkIndex)),
			// std::vector<size_t>(1, densityIndex),
			std::vector<Updater*>(1, new TieSwapUpdater(networkIndex))
			);

	attributeSaom.addEffectParameterIndexPair(new DensityEffect(networkIndex), densityIndex);

	attributeSaom.setDebug(false);


	for (int iSim = 0; iSim < nSimuations; iSim++){
		TieModelResult * result =
				dynamic_cast<TieModelResult *>(attributeSaom.getChange(processState));
		averageTies[result->getActorIndex1()][result->getActorIndex2()] +=
				1.0 / (double) nSimuations;
	}

	// print results
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){

			std::cout << i << "->" << j << ": " <<
					averageTies[i][j] << " (" <<
					tieProbabilities[i][j] << ")" << std::endl;

		}
	}

	// test results
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			ASSERT(fabs(tieProbabilities[i][j] - averageTies[i][j]) < allowedDeviation);
		}
	}


}



cute::suite getTestSaomSuite(){
	cute::suite s;

	s.push_back(CUTE(poissonparameterTest));
	s.push_back(CUTE(densityChoiceTest));
	s.push_back(CUTE(transitivityChoiceTest));
	s.push_back(CUTE(densityParameterSaomTest));
	// commented out as very time consuming (in original specification)
	s.push_back(CUTE(steglichParameterTest1));
	// s.push_back(CUTE(steglichParameterTest2));
	s.push_back(CUTE(attributeMultinomialChoiceNetworkChangeModelTest));


	return s;
}



#endif /* SAOMTEST_H_ */
