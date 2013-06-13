/*
 * ProcessStateManager.h
 *
 *  Created on: Nov 30, 2012
 *      Author: cws
 */

#ifndef PROCESSSTATEMANAGER_H_
#define PROCESSSTATEMANAGER_H_

#include <string>
#include "../network/Network.h"
#include "../attribute/AttributeContainer.h"
#include "../processstate/ProcessState.h"

/**
 * Wrapper for class ProcessState that allows named
 * access to elements.
 */
class ProcessStateManager{
public:
	ProcessStateManager(std::string name);

	size_t addNetwork(Network* network,
			std::string name);
	size_t addAttributeContainter(AttributeContainer* attributeContainer,
			std::string name);
	size_t addGlobalAttribute(double attribute,
			std::string name);

	Network* getNetwork(std::string name);
	AttributeContainer* getAttributeContainer(std::string name);
	double getGlobalAttribute(std::string name);

	size_t getNetworkIndex(std::string name);
	size_t getAttributeContainerIndex(std::string name);
	size_t getGlobalAttributeIndex(std::string name);

	ProcessState * getProcessState();

	std::string getName();

private:
	ProcessState* _processState;

	std::string _name;
	std::map<std::string, size_t> _networkNameIndexMap;
	std::map<std::string, size_t> _attributeContainerNameIndexMap;
	std::map<std::string, size_t> _globalAttributeNameIndexMap;

};


#endif /* PROCESSSTATEMANAGER_H_ */
