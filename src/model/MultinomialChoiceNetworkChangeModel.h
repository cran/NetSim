/*
 * MultinomialChoiceChangeModel.h
 *
 *  Created on: Dec 9, 2012
 *      Author: cws
 */

#ifndef MULTINOMIALCHOICENETWORKCHANGEMODEL_H_
#define MULTINOMIALCHOICENETWORKCHANGEMODEL_H_

#include <utility>
#include <math.h>
#include <iostream>
#include "MultinomialChoiceUtils.h"
#include "ChangeModel.h"
#include "saom/SaomEffect.h"
#include "Updater.h"
#include "ModelResult.h"
#include "../utils/Random.h"
#include <stdexcept>
#include "../io/Output.h"

class MultinomialChoiceNetworkChangeModel : public ChangeModel{

public:
	/**
	 * Create a simple model in which choices are based on how
	 * the network would look like after a number of updates
	 */
	MultinomialChoiceNetworkChangeModel(int actorIndex,
			size_t dependentNetworkIndex,
			std::set<std::pair<SaomEffect*, double> *> * effectParameterPairs,
			std::vector<Updater*>* updaters);

	/**
	 * create a multinomial choice model in which the choice
	 * dependes on a process state without changes
	 */
	MultinomialChoiceNetworkChangeModel(int actorIndex,
			size_t dependentNetworkIndex,
			std::set<std::pair<SaomEffect*, double> *> * effectParameterPairs);


	/**
	 * get actor for which the change is to be applied.
	 */
	ModelResult* getChange(ProcessState* processState);

	/**
	 * If set, extra debug messages will be sent to the console
	 */
	void setDebugMode(bool verbose = true);

	void addEffectParameterPair(SaomEffect* effect, double parameter);


private:
	int _actorIndex;
	size_t _dependentNetworkIndex;
	std::set<std::pair<SaomEffect*, double> *> * _effectParameterPairs;
	std::vector<Updater*> * _updaters;
	bool _debug;

	void init(int actorIndex,
			size_t dependentNetworkIndex,
			std::set<std::pair<SaomEffect*, double> *> * effectParameterPairs,
			std::vector<Updater*>* updaters);
	void calculateTieContributionsWithMemento(int nActors,
			const std::vector<double>& objectiveFunctions, double denominator,
			ProcessState* processState);
};

class AttributeMultinomialChoiceNetworkChangeModel : public ChangeModel{

public:
	AttributeMultinomialChoiceNetworkChangeModel(
			size_t dependentNetworkIndex,
			size_t poissonAttributeIndex, // to determine focal actor
			// std::vector<SaomEffect*> saomEffects,
			// std::vector<size_t> individualSaomParameters,
			std::vector<Updater*> updaters);


	/**
	 * get focal actor and actor for
	 * which the change is to be applied.
	 */
	ModelResult* getChange(ProcessState* processState);

	void addEffectParameterIndexPair(SaomEffect* effect, size_t index);

private:
	size_t _poissonAttributeIndex;// to determine focal actor
	size_t _dependentNetworkIndex;
	std::vector<SaomEffect*> _saomEffects;
	std::vector<size_t> _saomParameterIndexes;
	std::vector<Updater*> _updaters;

};

class EmptyMultinomialChoiceNetworkChangeModel : public ChangeModel{

public:
	EmptyMultinomialChoiceNetworkChangeModel();

	ModelResult * getChange(ProcessState * processState);

};

// TODO do we need this class? Where sould it be located?
class MissSpecifiedModelException : public std::runtime_error{
public:
	MissSpecifiedModelException(const std::string& message)
        : std::runtime_error(message) { };
};


#endif /* MULTINOMIALCHOICENETWORKCHANGEMODEL_H_ */
