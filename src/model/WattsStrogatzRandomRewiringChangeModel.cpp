/*
 * WattsStrogatzRandomRewiringChangeModel.cpp
 *
 *  Created on: Feb 26, 2013
 *      Author: cws
 */

#include "WattsStrogatzRandomRewiringChangeModel.h"

WattsStrogatzRandomRewiringChangeModel::WattsStrogatzRandomRewiringChangeModel(
		size_t networkIndex) {
	_networkIndex = networkIndex;
	_debug = false;
}

ModelResult* WattsStrogatzRandomRewiringChangeModel::getChange(
		ProcessState* processState) {

	int actor1, actor2, actor3, actor4 = -1;

	MemoryOneModeNetwork * network = dynamic_cast<MemoryOneModeNetwork *>(
			processState->getNetwork(_networkIndex));
	std::set<int> actorIDs = network->getActorIDs();

	// find a random tie to dissolve

	std::pair<int, int> tieToDissolve = NetworkUtils::getRandomTie(network);

	actor1 = tieToDissolve.first;
	actor2 = tieToDissolve.second;
	actor3 = actor1;

	// find a random actor to connect to
	// who is not connected to actor1

	// no rewiring of that actor possible (already connected to every other node)
	if (network->getOutDegree(actor1) == (network->getSize() - 1)){
		if (_debug) Output() << "Node " << actor1 <<
				" is connected with all other nodes" << "\n";
		return new TwoTiesModelResult(actor1,
				actor2,
				actor1,
				actor2);
	}

	double randConnect = Random::getInstance().getRandom();
	int nNotConnectedNodes = network->getSize() - 1 - network->getOutDegree(actor1);
	int chosenActorNumber = randConnect * (double) nNotConnectedNodes;
	int actorNumber = 0;


	if (_debug) Output() << "nNotConnected = " << nNotConnectedNodes <<
			"; rand = " << randConnect << "\n";
	if (_debug) Output() << "Choose actor number " << chosenActorNumber << "\n";

	std::set<int>::iterator itActor = actorIDs.begin();
	for(; itActor != actorIDs.end(); ++itActor){
		int j = *itActor;
		if (!(network->hasTie(actor1, j)) && (actor1 != j) ){
			if (actorNumber == chosenActorNumber)
				actor4 = j;
			actorNumber++;
		}

	}

	if (actor1 < 0 ||
			actor2 < 0 ||
			actor3 < 0 ||
			actor4 < 0)
		throw std::invalid_argument("Illegal actor ID in Watts Strogatz random rewiring model");

	return new TwoTiesModelResult(
			actor1,
			actor2,
			actor3,
			actor4);

}

void WattsStrogatzRandomRewiringChangeModel::setDebug(bool debug) {
	_debug = debug;
}
