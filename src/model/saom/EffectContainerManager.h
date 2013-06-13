/*
 * EffectContainerManager.h
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#ifndef EFFECTCONTAINERMANAGER_H_
#define EFFECTCONTAINERMANAGER_H_

#include <utility>
#include <set>
#include "SaomEffect.h"
#include "../../io/Output.h"

class EffectContainerManager{

public:

	EffectContainerManager();

	std::set<std::pair<SaomEffect*, double> *> * getEffectContainer();

	/**
	 * add a new effect.
	 * TODO: checks whether this effect already exists
	 */
	void addToEffectContainer(
			SaomEffect * effect, double parameter);

	bool removeFromEffectContainer(SaomEffect * effect);


private:
	std::set<std::pair<SaomEffect*, double> *> _effectContainer;

};



#endif /* EFFECTCONTAINERMANAGER_H_ */
