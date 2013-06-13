/*
 * MultinomialChoiceNetworkChangeModel.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: cws
 */

#include "MultinomialChoiceNetworkChangeModel.h"

MultinomialChoiceNetworkChangeModel::MultinomialChoiceNetworkChangeModel(
		int actorIndex, size_t dependentNetworkIndex,
		std::set<std::pair<SaomEffect*, double> *>* effectParameterPairs,
		std::vector<Updater*>* updaters) {

	init(actorIndex, dependentNetworkIndex, effectParameterPairs, updaters);
}

MultinomialChoiceNetworkChangeModel::MultinomialChoiceNetworkChangeModel(
		int actorIndex, size_t dependentNetworkIndex,
		std::set<std::pair<SaomEffect*, double> *>* effectParameterPairs) {

	std::vector<Updater*>* updaters = new std::vector<Updater*>();
	init(actorIndex, dependentNetworkIndex, effectParameterPairs, updaters);
}

ModelResult* MultinomialChoiceNetworkChangeModel::getChange(
		ProcessState* processState) {
	// TODO this part can be more flexible
	// e.g. only use "active" actors as potential receivers
	std::set<int> actorIDs = processState->getActorIds();
	std::set<int>::iterator itActors;
	std::map<int, int> indexIdMap;
	int nActors = actorIDs.size();

	std::vector<double> objectiveFunctions(nActors, 0);
	double denominator = 0;
	// DEPRECATED
	// updates objectiveFunctions and denominators
	// calculateTieContributionsWithMemento(nActors,
	//		objectiveFunctions, denominator, processState);
	// calculate effects for the current networks (un-unchanged)
	int nEffects = _effectParameterPairs->size();
	std::vector<std::pair<double, double> > statisticsAndParametersWithoutChange(
			nEffects, std::pair<double, double>(0, 0));
	std::vector<std::vector<double> > statisticsWithoutTieContribution(
			nActors, std::vector<double>(nEffects, 0));
	// calculate statistics
	int effectIndex = 0;
	std::set<std::pair<SaomEffect*, double> *>::iterator itEffects =
			_effectParameterPairs->begin();
	for (; itEffects != _effectParameterPairs->end(); ++itEffects) {
		int i = _actorIndex;
		// calculate effects without changes
		// save effect statistic
		double statistic = (*itEffects)->first->getEffect(processState,
				_actorIndex);
		statisticsAndParametersWithoutChange[effectIndex].first +=
				statistic;
		// save effect parameter (beta)
		double parameter = (*itEffects)->second;
		statisticsAndParametersWithoutChange[effectIndex].second =
				parameter;

		// iterate over all actors
		itActors = actorIDs.begin();
		int actorIndex = 0; // the internal array index pointing to  the actor ID
		for (; itActors != actorIDs.end(); ++itActors) {
			int j = *itActors;
			// in the first iteration do create the map
			if (effectIndex == 0)
				indexIdMap[actorIndex] = j;

			NetworkEffect* effect =
					dynamic_cast<NetworkEffect*>((*itEffects)->first);
			double contribution = effect->getEffectContribution(
					processState, i, j);
			Network* net = processState->getNetwork(_dependentNetworkIndex);
			// TODO: Here, we assume that an update is a tie swap!
			if (net->hasTie(i, j)) {
				statisticsWithoutTieContribution[actorIndex][effectIndex] =
						statistic - contribution;
				if (_debug)
					Output() << "Tie removal from " << i << " to " << j
							<< " contributes " << contribution << "\n";
			} else {
				// i->j does not exist or i == j
				if (i != j) {
					statisticsWithoutTieContribution[actorIndex][effectIndex] =
							statistic + contribution;
					if (_debug)
						Output() << "Tie insertion from " << i << " to "
								<< j << " contributes " << contribution
								<< "\n";
				} else // choosing oneself means changing nothing
				{
					statisticsWithoutTieContribution[actorIndex][effectIndex] =
							statistic;
				}
			}
			objectiveFunctions[actorIndex] +=
					statisticsWithoutTieContribution[actorIndex][effectIndex]
							* parameter;
			actorIndex++;
		} // iterate over all actors
		++effectIndex;
	}

	// calculate denominator
	for (size_t i = 0; i < objectiveFunctions.size(); i++) {
		denominator += exp(objectiveFunctions[i]);
		if (_debug)
			Output() << "o.f. of actor choice " << i << ": "
					<< objectiveFunctions[i] << "\n";
	}

	if (_debug)
		Output() << "Active actor: " << _actorIndex << "\n";

	if (_debug)
		Output() << "calculated denominator: " << denominator << "\n";

	if (denominator >= DBL_MAX)
		throw MissSpecifiedModelException(
				"Infinite denominator in SAOM. You probably misspecified your model.");

	// draw random number
	double randomNumber = Random::getInstance().getRandom();
	// iterate over objective functions until actor is found
	double cumProbability = 0;
	for (int actorIndex = 0; actorIndex < nActors; actorIndex++) {
		double probability = exp(objectiveFunctions[actorIndex]) / denominator;
		cumProbability += probability;
		// indexIdMap[k] is the randomly chosen actor
		// create and return ActorModelResult
		if (cumProbability >= randomNumber) {
			// TieModelResult result(_actorIndex,j);
			return new TieModelResult(_actorIndex, indexIdMap[actorIndex]);
			// return &_tieModelResult;
		}
	}
	// Should never be reached except an error occurs
	Output() << "Should not be here MultinomialChoiceModel wrong return type";
	return new TieModelResult(-1, -1);
}

void MultinomialChoiceNetworkChangeModel::addEffectParameterPair(
		SaomEffect* effect, double parameter) {
	_effectParameterPairs->insert(
			new std::pair<SaomEffect*, double>(effect, parameter));
}

void MultinomialChoiceNetworkChangeModel::calculateTieContributionsWithMemento(
		int nActors, const std::vector<double>& objectiveFunctions,
		double denominator, ProcessState* processState) {
/*

	// save process state as memento
	ProcessStateMemento* memento = processState->saveToMemento();

	// iterate over all actors
	for (int j = 0; j < nActors; j++) {
		int i = _actorIndex;
		// update tie.
		for (std::vector<Updater*>::iterator it = _updaters->begin();
				it != _updaters->end(); ++it) {
			TieModelResult* result = new TieModelResult(i, j);
			(*it)->update(processState, result);
			delete result;
		}
		// calculate and save objective function
		objectiveFunctions[j] =
				MultinomialChoiceUtils::getValueObjectiveFunction(processState,
						i, _effectParameterPairs, _debug);
		// increase sum (denominator)
		denominator += exp(objectiveFunctions[j]);
		if (_debug)
			std::cout << "" << objectiveFunctions[j] << std::endl;

		// reset process state
		processState->restoreFromMemento(memento);
	} // iterate over all actors
	delete memento;
*/
}

void MultinomialChoiceNetworkChangeModel::setDebugMode(bool verbose) {
	_debug = verbose;
}

void MultinomialChoiceNetworkChangeModel::init(int actorIndex,
		size_t dependentNetworkIndex,
		std::set<std::pair<SaomEffect*, double> *>* effectParameterPairs,
		std::vector<Updater*>* updaters) {

	_actorIndex = actorIndex;
	_dependentNetworkIndex = dependentNetworkIndex;
	_effectParameterPairs = effectParameterPairs;
	_updaters = updaters;
	_debug = false;

}

EmptyMultinomialChoiceNetworkChangeModel::EmptyMultinomialChoiceNetworkChangeModel() {
}

ModelResult* EmptyMultinomialChoiceNetworkChangeModel::getChange(
		ProcessState* processState) {
	return new ActorModelResult(-1);
}

AttributeMultinomialChoiceNetworkChangeModel::AttributeMultinomialChoiceNetworkChangeModel(
		size_t dependentNetworkIndex,
		size_t jointPoissonAttributeIndex,
		// std::vector<SaomEffect*> saomEffects,
		// std::vector<size_t> individualSaomParameters,
		std::vector<Updater*> updaters) {

	//if (saomEffects.size() != individualSaomParameters.size())
	//	throw std::invalid_argument(
	//			"saomEffects and individualSaomParameters not of same length");

	_poissonAttributeIndex = jointPoissonAttributeIndex;
	_dependentNetworkIndex = dependentNetworkIndex;
	// std::vector<SaomEffect*> _saomEffects;
	// std::vector<size_t> _saomParameterIndexes;
	_updaters = updaters;
	_debug = false;
}

ModelResult* AttributeMultinomialChoiceNetworkChangeModel::getChange(
		ProcessState* processState) {

	// determine focal actor using (individual) Poisson parameter

	int focalActor = -1;

	std::set<int> ids = processState->getActorIds();
	AttributeContainer * poissonAttributeContainer =
			processState->getAttributeContainer(_poissonAttributeIndex);

	double sumPoissonParameters = 0;
	std::set<int>::iterator itIds = ids.begin();
	for(; itIds != ids.end(); ++itIds){
		double poissonParameter = poissonAttributeContainer->getValue(*itIds);
		sumPoissonParameters += poissonParameter;
	}

	double rand = Random::getInstance().getRandom();
	double cumulatedProbability = 0;
	itIds = ids.begin();
	for(; itIds != ids.end(); ++itIds){
		double poissonParameter = poissonAttributeContainer->getValue(*itIds);
		cumulatedProbability += poissonParameter / sumPoissonParameters;

		if (cumulatedProbability >= rand){
			focalActor = *itIds;
			break;
		}
	}

	if (_debug) Output() << "Actor " << focalActor << " chosen." << "\n";


	// prepare parameters for saom constructor

	std::set<std::pair<SaomEffect*, double> *> effectParameterPairs;

	for (int i = 0; i < _saomEffects.size(); i++){
		size_t index = _saomParameterIndexes[i];
		AttributeContainer * paramContainer =
				processState->getAttributeContainer(index);
		double param = paramContainer->getValue(focalActor);
		effectParameterPairs.insert(new std::pair<SaomEffect*, double>(
				_saomEffects[i], param));
	}

	// create saom with individual parameters

	MultinomialChoiceNetworkChangeModel * saom =
			new MultinomialChoiceNetworkChangeModel(
					focalActor, _dependentNetworkIndex,
					&effectParameterPairs, &_updaters);
	saom->setDebugMode(_debug);

	// run get change function in new saom

	ModelResult * result = saom->getChange(processState);

	// delete saom

	delete saom;

	// return model result

	return result;
}

void AttributeMultinomialChoiceNetworkChangeModel::addEffectParameterIndexPair(
		SaomEffect* effect, size_t index) {
	_saomEffects.push_back(effect);
	_saomParameterIndexes.push_back(index);
}
