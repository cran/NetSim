/*
 * Network.h
 *
 *  Created on: Nov 28, 2012
 *      Author: cws
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdlib.h>

class NetworkMemento{
public:
	virtual ~NetworkMemento(){ }
};



/**
 * Abstract network class
 */
class Network{

public:

	// the following functions are abstract

	virtual ~Network() {}
	virtual bool addTie(int i, int j) = 0;
	virtual bool setTie(int i, int j, double value) = 0;
	virtual bool hasTie(int i, int j) = 0;
	virtual double getTieValue(int i, int j) = 0;
	virtual bool removeTie(int i, int j) = 0;
	virtual int getSize() = 0;
	virtual void addActor(size_t id) = 0;
	virtual void deleteActor(size_t id) = 0;

	// virtual bool hasActor(int i) = 0;
	// virtual bool removeActor(int i) = 0;
	// virtual bool includeActor(int i) = 0;
	// virtual int includeActor() = 0;
	// virtual iterator getActorIterator() = 0;

};


#endif /* NETWORK_H_ */
