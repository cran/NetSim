/*
 * MultinomialChoiceUtils.cpp
 *
 *  Created on: Jan 27, 2013
 *      Author: cws
 */

#include "MultinomialChoiceUtils.h"


/**
 * returns objective function of a multinomial choice model
 * This objective function is not yet exponentialized.
 */
double MultinomialChoiceUtils::getValueObjectiveFunction(
		ProcessState* processState, int focalActor,
		std::set<std::pair<SaomEffect*, double> *>* effectParameterPairs,
		bool debug) {

			double valueObjectiveFunction = 0.0;

			std::set<std::pair<SaomEffect*, double> *>::iterator itEffects = effectParameterPairs->begin();

			for (; itEffects != effectParameterPairs->end(); ++itEffects){
				double effectValue = (*itEffects)->first->getEffect(processState, focalActor);
				double parameter = (*itEffects)->second;

				if(debug) Output() << "  param=" << parameter
						<< " value=" << effectValue << "\n";

				valueObjectiveFunction += ( effectValue * parameter );
			}


			return valueObjectiveFunction;
}
