/*
 * TimeModel.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "TimeModel.h"

PoissonTimeModel::PoissonTimeModel(double poissonParameter) {
	_poissonParameter = poissonParameter;
}

double PoissonTimeModel::getTimeSpan(ProcessState* processState) {
	double randomNumber = Random::getInstance().getRandom();
	double timeSpan = -1 * log(1 - randomNumber) / _poissonParameter;
	return timeSpan;
}

double PoissonTimeModel::getParameter() {
	return _poissonParameter;
}

RoundBasedTimeModel::RoundBasedTimeModel(size_t timerIndex,
		double intervalLength,
		double startTime) {
	_intervalLength = intervalLength;
	_timerIndex = timerIndex;
	_startTime = startTime;
}

double RoundBasedTimeModel::getTimeSpan(ProcessState* processState) {
	double time = processState->getGlobalAttribute(_timerIndex);

	if (time < _startTime) return (_startTime - time);

	else {
		double timeSinceLast = fmod(time - _startTime, _intervalLength);
		return _intervalLength - timeSinceLast;
	}
}

JointAttributePoissonTimeModel::JointAttributePoissonTimeModel(
		size_t attributeContainerIndex) {
	_attributeContainerIndex = attributeContainerIndex;
}

double JointAttributePoissonTimeModel::getTimeSpan(ProcessState* processState) {

	AttributeContainer * container = processState->getAttributeContainer(
			_attributeContainerIndex);

	std::set<int> ids = processState->getActorIds();

	double jointPoissonParameter = 0;

	std::set<int>::iterator itIds = ids.begin();
	for(; itIds != ids.end(); ++itIds)
		jointPoissonParameter += container->getValue(*itIds);

	double randomNumber = Random::getInstance().getRandom();
	double timeSpan = -1 * log(1 - randomNumber) / jointPoissonParameter;
	return timeSpan;
}
