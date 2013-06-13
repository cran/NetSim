/*
 * ProcessStateManager.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: cws
 */

# include "ProcessStateManager.h"

ProcessStateManager::ProcessStateManager(std::string name) {
	_processState = new ProcessState();
	_name = name;
}

size_t ProcessStateManager::addNetwork(Network* network, std::string name) {
	// name has previously been used
	if (_networkNameIndexMap.find(name) != _networkNameIndexMap.end()){
		// TODO remove network from _processState \
		// (needs to be implemented with safe index values)
	}
	size_t index = _processState->addNetwork(network);
	_networkNameIndexMap.insert( std::pair<std::string, size_t>(name, index) );
	return index;
}

size_t ProcessStateManager::addAttributeContainter(
		AttributeContainer* attributeContainer, std::string name) {
	size_t index = _processState->addAttributeContainer(attributeContainer);
	_attributeContainerNameIndexMap.insert(
			std::pair<std::string, size_t>(name, index));
	return index;
}

size_t ProcessStateManager::addGlobalAttribute(double attribute,
		std::string name) {
	size_t index = _processState->addGlobalAttribute(attribute);
	_globalAttributeNameIndexMap.insert(
			std::pair<std::string, size_t>(name, index));
	return index;
}

Network* ProcessStateManager::getNetwork(std::string name) {
	size_t index = _networkNameIndexMap[name];
	return _processState->getNetwork(index);
}

AttributeContainer* ProcessStateManager::getAttributeContainer(
		std::string name) {
	size_t index = _attributeContainerNameIndexMap[name];
	return _processState->getAttributeContainer(index);
}

double ProcessStateManager::getGlobalAttribute(std::string name) {
	size_t index = _globalAttributeNameIndexMap[name];
	return _processState->getGlobalAttribute(index);
}

size_t ProcessStateManager::getNetworkIndex(std::string name) {
	return _networkNameIndexMap[name];
}

size_t ProcessStateManager::getAttributeContainerIndex(std::string name) {
	return _attributeContainerNameIndexMap[name];
}

size_t ProcessStateManager::getGlobalAttributeIndex(std::string name) {
	return _globalAttributeNameIndexMap[name];
}

ProcessState * ProcessStateManager::getProcessState() {
	return _processState;
}

std::string ProcessStateManager::getName() {
	return _name;
}
