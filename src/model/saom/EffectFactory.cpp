/*
 * EffectFactory.cpp
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#include "EffectFactory.h"

std::string EffectFactory::getType(std::string name) {
	// create dummy objects
	if(NULL != getOneModeNetworkEffect(name, -1))
		return "OneModeNetworkEffect";

	if(NULL != getAttributeOneModeNetworkEffect(name, -1, -1))
		return "AttributeOneModeNetworkEffect";

	if(NULL != getSimilarityAttributeOneModeNetworkEffect(name, -1, -1, 0))
		return "SimilarityAttributeOneModeNetworkEffect";

	if(NULL != getAttributeEffect(name, -1))
		return "AttributeEffect";

	if(NULL != getMultiplexNetworkEffect(name, -1, -1))
		return "MultiplexNetworkEffect";


	return "UnknownType";

}

NetworkEffect * EffectFactory::getOneModeNetworkEffect(
		std::string name, size_t networkIndex) {

	// effects ordered according to the SIENA manual

	if(name == "density")
		return new DensityEffect(networkIndex);

	if(name == "recip")
		return new ReciprocityEffect(networkIndex);

	if(name == "transTrip")
		return new TransitivityEffect(networkIndex);

	if(name == "cycle3")
		return new ThreeCycleEffect(networkIndex);

	if(name == "nbrDist2")
		return new TwoPathEffect(networkIndex);

	if(name == "inPop")
		return new InPopularityEffect(networkIndex);

	if(name == "outPop")
		return new OutPopularityEffect(networkIndex);


	return NULL;

	// error moved to R
	// std::string errorMessage = "Unknown one mode network effect: ";
	// errorMessage += name;
	// throw std::invalid_argument(errorMessage);
}

AttributeOneModeNetworkEffect* EffectFactory::getAttributeOneModeNetworkEffect(
		std::string name,
		size_t attributeIndex,
		size_t networkIndex) {

	if(name == "altX")
		return new AltXEffect(attributeIndex, networkIndex);

	if(name == "egoX")
		return new EgoXEffect(attributeIndex, networkIndex);

	return NULL;

}

SimilarityAttributeOneModeNetworkEffect* EffectFactory::getSimilarityAttributeOneModeNetworkEffect(
		std::string name,
		size_t attributeIndex,
		size_t networkIndex,
		double meanSimilarityScores) {

	// simX and totSim share the same change model
	if(name == "simX")
		return new TotalSimilarityEffect(attributeIndex, networkIndex, meanSimilarityScores);

	// simX and totSim share the same change model
	if(name == "totSim")
		return new TotalSimilarityEffect(attributeIndex, networkIndex, meanSimilarityScores);

	return NULL;
}

AttributeEffect * EffectFactory::getAttributeEffect(std::string name,
		size_t attributeIndex) {

	if(name == "linear")
		return new LinearShapeAttributeEffect(attributeIndex);


	if(name == "quad")
		return new QuadraticShapeAttributeEffect(attributeIndex);

	return NULL;

}

MultiplexNetworkEffect* EffectFactory::getMultiplexNetworkEffect(
		std::string name, size_t networkIndex, size_t secondNetworkIndex) {

	if(name == "crprod")
		return new DyadicCovariateXEffect(networkIndex, secondNetworkIndex, 0);

	return NULL;
}
