/*
 * EffectFactory.h
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#ifndef EFFECTFACTORY_H_
#define EFFECTFACTORY_H_

#include "SaomEffect.h"
#include <stdexcept>
#include <string>

/**
 * Factory to create SAOM effects by name.
 * Names are according to the naming conventions
 * in SIENA (see SIENA manual p. 100ff).
 * Class manages the names of effect types.
 */
class EffectFactory{

public:

	/**
	 * Get name of effect type for later processing in R.
	 */
	std::string getType(std::string name);

	NetworkEffect * getOneModeNetworkEffect(
			std::string name,
			size_t networkIndex);

	AttributeOneModeNetworkEffect * getAttributeOneModeNetworkEffect(
			std::string name,
			size_t attributeIndex,
			size_t networkIndex);

	SimilarityAttributeOneModeNetworkEffect * getSimilarityAttributeOneModeNetworkEffect(
			std::string name,
			size_t attributeIndex,
			size_t networkIndex,
			double meanSimilarityScores);

	AttributeEffect * getAttributeEffect(
			std::string name,
			size_t attributeIndex);

	MultiplexNetworkEffect * getMultiplexNetworkEffect(
			std::string name,
			size_t networkIndex,
			size_t secondNetworkIndex);


};


#endif /* EFFECTFACTORY_H_ */
