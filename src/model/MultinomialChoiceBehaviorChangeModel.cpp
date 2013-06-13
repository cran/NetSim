/*
 * MultinomialChoiceBehaviorChangeModel.cpp
 *
 *  Created on: Jan 27, 2013
 *      Author: cws
 */

#include "MultinomialChoiceBehaviorChangeModel.h"

MultinomialChoiceBehaviorChangeModel::MultinomialChoiceBehaviorChangeModel(
		int actorIndex, int attributeIndex,
		std::set<std::pair<SaomEffect*, double> *>* effectParameterPairs) {

	_actorIndex = actorIndex;
	_attributeIndex = attributeIndex;
	_effectParameterPairs = effectParameterPairs;

	_debug = false;

}

ModelResult* MultinomialChoiceBehaviorChangeModel::getChange(
		ProcessState* processState) {

	// make a set of allowed behaviors
	// current, (current - by) and (current + by)
	std::set<double> behaviorOptions;

	AttributeContainer * attributeContainer =
			processState->getAttributeContainer(_attributeIndex);

	double currentValue = attributeContainer->getValue(_actorIndex);
	double min = attributeContainer->getMin();
	double max = attributeContainer->getMax();
	double by = attributeContainer->getBy();

	behaviorOptions.insert(currentValue);
	if ((currentValue - by) >= min) behaviorOptions.insert(currentValue - by);
	if ((currentValue + by) <= max) behaviorOptions.insert(currentValue + by);

	// iterate over all options and create the denominator
	std::set<double>::iterator it = behaviorOptions.begin();

	std::vector<double> expObjectiveFunctions(behaviorOptions.size(), 0);
	std::vector<double> behaviorValues(behaviorOptions.size(), 0);
	double denominator = 0;
	size_t i = 0;
	for (; it != behaviorOptions.end(); ++it){

		// change behavior value to (*it)
		attributeContainer->setValue(_actorIndex, *it);

		double currentObjectiveFunction =
				MultinomialChoiceUtils::getValueObjectiveFunction(
						processState, _actorIndex, _effectParameterPairs, _debug);
		expObjectiveFunctions[i] = exp(currentObjectiveFunction);
		behaviorValues[i] = *it;
		denominator += exp(currentObjectiveFunction);

		i++;
	}

	// reset to original value
	attributeContainer->setValue(_actorIndex, currentValue);

	// draw the choice
	double cumulatedProbability = 0;
	for (i = 0; i < expObjectiveFunctions.size(); i++){

		cumulatedProbability += expObjectiveFunctions[i]/denominator;
		double rand = Random::getInstance().getRandom();

		if (cumulatedProbability > rand){
			_valueModelResult = new ValueModelResult(behaviorValues[i]);
			return _valueModelResult;
			// return new ValueModelResult(behaviorValues[i]);
		}
	}

	// Should never be reached except if an error occurs
	Output() << "Should not be here MultinomialBehaviorChoiceModel wrong return type";
	return new ValueModelResult(-1);



}
