/*
 * MultinomialChoiceBehaviorChangeModel.h
 *
 *  Created on: Jan 27, 2013
 *      Author: cws
 */

#ifndef MULTINOMIALCHOICEBEHAVIORCHANGEMODEL_H_
#define MULTINOMIALCHOICEBEHAVIORCHANGEMODEL_H_

#include "ChangeModel.h"
#include "ModelResult.h"
#include "saom/SaomEffect.h"
#include "../utils/Random.h"
#include "MultinomialChoiceUtils.h"
#include <utility>
#include <set>
#include <vector>
#include "../io/Output.h"


class MultinomialChoiceBehaviorChangeModel : public ChangeModel{

public:
	MultinomialChoiceBehaviorChangeModel(
			int actorIndex,
			int attributeIndex,
			std::set<std::pair<SaomEffect*, double> *> * effectParameterPairs);

	ModelResult * getChange(ProcessState * processState);

private:
	ValueModelResult * _valueModelResult;
	int _actorIndex;
	int _attributeIndex;
	bool _debug;
	std::set<std::pair<SaomEffect*, double> *> * _effectParameterPairs;
};


#endif /* MULTINOMIALCHOICEBEHAVIORCHANGEMODEL_H_ */
