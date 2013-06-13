/*
 * MultinomialChoiceUtils.h
 *
 *  Created on: Jan 27, 2013
 *      Author: cws
 */

#ifndef MULTINOMIALCHOICEUTILS_H_
#define MULTINOMIALCHOICEUTILS_H_

#include <utility>
#include <set>
#include "saom/SaomEffect.h"
#include "../io/Output.h"

class MultinomialChoiceUtils{

public:
	static double getValueObjectiveFunction(
			ProcessState* processState,
			int focalActor,
			std::set<std::pair<SaomEffect*, double> *> * effectParameterPairs,
			bool debug = false);
};



#endif /* MULTINOMIALCHOICEUTILS_H_ */
