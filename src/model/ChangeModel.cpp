/*
 * ChangeModel.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "ChangeModel.h"

ModelResult* NullChangeModel::getChange(ProcessState* processState) {
	return new ActorModelResult(0);
}

ChooseRandomTieChangeModel::ChooseRandomTieChangeModel(size_t networkIndex) {
	_networkIndex = networkIndex;
}

ModelResult* ChooseRandomTieChangeModel::getChange(ProcessState* processState) {

	MemoryOneModeNetwork * network = dynamic_cast<MemoryOneModeNetwork *>(
			processState->getNetwork(_networkIndex));

	std::pair<int, int> tie = NetworkUtils::getRandomTie(network);

	return new TieModelResult(tie.first, tie.second);
}

void ChangeModel::setDebug(bool debug) {
	_debug = debug;
}
