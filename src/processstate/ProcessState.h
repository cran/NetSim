/*
 * ProcessState.h
 *
 *  Created on: Nov 28, 2012
 *      Author: cws
 */

#ifndef PROCESSSTATE_H_
#define PROCESSSTATE_H_

#include <map>
//#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include "../network/Network.h"
#include "../attribute/AttributeContainer.h"
#include "../network/MemoryOneModeNetwork.h"


/*
 * forward declarations
 */
class ProcessStateMemento;

/**
 * Defines the process state of the simulated Markov process.
 * A ProcessState is a contains three vector of pointers to Networks,
 * AttributeContainer's and global attributes. Objects are addressed by index.
 */
class ProcessState{

public:

	ProcessState();

	/**
	 * Add a network pointer to the vector of network pointers
	 * @returns unique index
	 */
	size_t addNetwork(Network* network);

	/**
	 * Add a pointer to an AttributeContainter to the vector of network pointers
	 * @returns unique index
	 */
	size_t addAttributeContainer(AttributeContainer* attributeContainer);

	/**
	 * Adds a global attribute
	 * @returns unique index
	 */
	size_t addGlobalAttribute(double attribute);

	/**
	 * Resets an existing network pointer
	 */
	void setGlobalAttribute(size_t index, double attribute);

	/**
	 * Get network from network stack by index
	 */
	Network* getNetwork(size_t index);

	/**
	 * Get AttributeContainter from attribute container stack by index
	 */
	AttributeContainer* getAttributeContainer(size_t index);

	/**
	 * Get global attribute from attribute stack by index
	 */
	double getGlobalAttribute(size_t index);

	/**
	 * Get the number of actors represented by the elements of
	 * the process state
	 */
	int getNumberOfActors();

	// TODO: getActorIterator

	// TODO: implement
	int addActor();

	// TODO: remove actor
	void deleteActor(int id);

	/**
	 * returns the newest actor ID (maximum id)
	 * or -1 of the process state has no actors
	 */
	int getNewestActorIndex();

	/**
	 * get number of networks
	 */
	int getNumberOfNetworks();

	/**
	 * get number of attribute containers
	 */
	int getNumberOfAttributeContainers();

	/**
	 * get number of global attributes
	 */
	int getNumberOfGlobalAttributes();

	/**
	 * get all actor ids
	 */
	std::set<int> getActorIds();


	/**
	 * save process state as memento
	 */
	ProcessStateMemento * saveToMemento();

	/**
	 * restore process state from memento
	 */
	void restoreFromMemento(ProcessStateMemento * memento);


private:
	std::map<int, Network*> _networks;
	std::map<int, AttributeContainer*> _attributeContainers;
	std::map<int, double> _globalAttributes;
	size_t _nNetworks;
	size_t _nAttributeContainers;
	size_t _nGlobalAttributes;
	size_t _nActors;

	int _maxActorID;

	// assumed to be an ordered container in some sub functions
	std::set<int> _ids;

};

class ProcessStateMemento{
private:

	void setNetworkMemento(int i, NetworkMemento* memento);
	void setAttributeContainerMemento(int i, AttributeContainerMemento* memento);
	void setGlobalAttribute(int i, double);
	NetworkMemento* getNetworkMemento(int i);
	AttributeContainerMemento* getAttributeContainerMemento(int i);
	double getGlobalAttribute(int i);

	std::map<int, NetworkMemento*> _networkMementos;
	std::map<int, AttributeContainerMemento*> _attributeContainerMementos;
	std::map<int, double> _globalAttributes;

	friend class ProcessState;
};




#endif /* PROCESSSTATE_H_ */
