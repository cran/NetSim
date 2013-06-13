/*
 * RInstanceFactory.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: cws
 */

#include "RInstanceFactory.h"

RInstanceFactory& RInstanceFactory::getInstance() {
    // The only instance
    // Guaranteed to be lazy initialized
    // Guaranteed that it will be destroyed correctly
    static RInstanceFactory instance;
    return instance;

}

RInside RInstanceFactory::getRInside() {
	return _R;
}

RInstanceFactory::RInstanceFactory() {

}
