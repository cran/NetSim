/*
 * RInstanceFactory.h
 *
 *  Created on: Jan 21, 2013
 *      Author: cws
 */

#ifndef RINSTANCEFACTORY_H_
#define RINSTANCEFACTORY_H_


#include <RInside.h>

/**
 * Provides a unique RInside instance.
 * The class is implemented as a Singleton as
 * globally only one RInside instance is allowed
 * to be created.
 */
class RInstanceFactory{

public:
	static RInstanceFactory& getInstance();

	RInside getRInside();

private:
	RInstanceFactory();
    // Stop the compiler generating methods or copy the object
	RInstanceFactory(RInstanceFactory const& copy);            // Not Implemented
	RInstanceFactory& operator=(RInstanceFactory const& copy); // Not Implemented

	RInside _R;
};

#endif /* RINSTANCEFACTORY_H_ */
