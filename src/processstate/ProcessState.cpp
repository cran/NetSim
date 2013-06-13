/*
 * ProcessState.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: cws
 */

#include "ProcessState.h"

ProcessState::ProcessState() {
	_nActors = 0;
	_nNetworks = 0;
	_nAttributeContainers = 0;
	_nGlobalAttributes = 0;
	_maxActorID = -1;
	// vectors are initialized automatically
}


size_t ProcessState::addNetwork(Network* network) {

	if (network->getSize() > _nActors){
		_nActors = network->getSize();
		_maxActorID = _nActors - 1;
		for (int i = 0; i < _nActors; i++)
			_ids.insert(i);
	}

	_networks[_nNetworks] = network;
	return _nNetworks++;

	// _networks.insert(_networks.end(), network);
	// return -1 + _networks.end()-_networks.begin();
}

size_t ProcessState::addAttributeContainer(AttributeContainer* attributeContainer) {
	if (attributeContainer->getSize() > _nActors){
		_nActors = attributeContainer->getSize();
		_maxActorID = _nActors - 1;
		for (int i = 0; i < _nActors; i++)
			_ids.insert(i);
	}

	_attributeContainers[_nAttributeContainers] = attributeContainer;
	return _nAttributeContainers++;

	// _attributeContainers.insert(_attributeContainers.end(), attributeContainer);
	// return -1 + _attributeContainers.end() - _attributeContainers.begin();
}

size_t ProcessState::addGlobalAttribute(double attribute) {

	_globalAttributes[_nGlobalAttributes] = attribute;
	return _nGlobalAttributes++;

	// _globalAttributes.insert(_globalAttributes.end(), attribute);
	// return -1 + _globalAttributes.end() - _globalAttributes.begin() ;
}

void ProcessState::setGlobalAttribute(size_t index, double attribute) {
	_globalAttributes[index] = attribute;
}

Network* ProcessState::getNetwork(size_t index) {
	if ((index >= _nNetworks))
		throw std::invalid_argument("Network index does not exist: "
				// TODO add a proper error string
				// (removed because of problems with g++ boost headers in Windows)
		//		+ boost::lexical_cast<std::string>(index)
				);
	return _networks[index];
}

AttributeContainer* ProcessState::getAttributeContainer(size_t index) {
	if ((index >= _nAttributeContainers))
		throw std::invalid_argument("Attribute container index does not exist: "
		//		+ boost::lexical_cast<std::string>(index)
				);
	return _attributeContainers[index];
}


double ProcessState::getGlobalAttribute(size_t index) {
	if ((index >= _nGlobalAttributes))
		throw std::invalid_argument("Global attribute index does not exist: "
		//		+ boost::lexical_cast<std::string>(index)
				);
	return _globalAttributes[index];
}


int ProcessState::getNumberOfActors() {
	return _nActors;
}

int ProcessState::getNumberOfNetworks() {
	return _nNetworks;
}

int ProcessState::getNumberOfAttributeContainers() {
	return _nAttributeContainers;
}

int ProcessState::getNumberOfGlobalAttributes() {
	return _nGlobalAttributes;
}

ProcessStateMemento* ProcessState::saveToMemento() {
	ProcessStateMemento * memento = new ProcessStateMemento();

	// save networks
	std::map<int, Network*>::iterator itNet = _networks.begin();
	for (; itNet != _networks.end(); ++itNet){
		// first: index
		int index = (*itNet).first;
		// second: network pointer
		Network * network = (*itNet).second;

		MemoryOneModeNetwork * memoryNet =
				dynamic_cast<MemoryOneModeNetwork*>(network);
		NetworkMemento * networkMemento = memoryNet->saveToMemento();
		memento->setNetworkMemento(index, networkMemento);
	}

	// save attribute containers
	std::map<int, AttributeContainer*>::iterator itAttCont = _attributeContainers.begin();
	for (; itAttCont != _attributeContainers.end(); ++itAttCont){
		memento->setAttributeContainerMemento((*itAttCont).first,
				(*itAttCont).second->saveToMemento());
	}

	// save global attributes
	std::map<int, double>::iterator itGlobAtt = _globalAttributes.begin();
	for (; itGlobAtt != _globalAttributes.end(); ++itGlobAtt){
		_globalAttributes[(*itGlobAtt).first] = (*itGlobAtt).second;
	}

	return memento;
}

int ProcessState::addActor() {
	int newID = _maxActorID + 1;
	_maxActorID = newID;
	_ids.insert(newID);
	_nActors++;

	std::map<int, Network*>::iterator itNetworks =  _networks.begin();
	for (; itNetworks != _networks.end(); ++itNetworks){
		(*itNetworks).second->addActor(newID);
	}


	std::map<int, AttributeContainer*>::iterator itAttributes =
			_attributeContainers.begin();
	for (; itAttributes != _attributeContainers.end(); ++itAttributes){
		(*itAttributes).second->addActor(newID);
	}


	return newID;
}

int ProcessState::getNewestActorIndex() {
	return _maxActorID;
}

void ProcessState::deleteActor(int id) {

	// delete from all attribute containers and networks
	std::map<int, Network*>::iterator itNetworks =  _networks.begin();
	for (; itNetworks != _networks.end(); ++itNetworks){
		(*itNetworks).second->deleteActor(id);
	}


	std::map<int, AttributeContainer*>::iterator itAttributes =
			_attributeContainers.begin();
	for (; itAttributes != _attributeContainers.end(); ++itAttributes){
		(*itAttributes).second->deleteActor(id);
	}


	_ids.erase(id);
	int newMaxID = -1;

	// sets are ordered
	// use right-most id as maximum ID
	if(!_ids.empty())
		newMaxID = *(_ids.rbegin());

	_maxActorID = newMaxID;
	_nActors--;

}

std::set<int> ProcessState::getActorIds() {
	return _ids;
}

void ProcessState::restoreFromMemento(ProcessStateMemento* memento) {

	// restore networks
	std::map<int, Network*>::iterator itNet = _networks.begin();
	for (; itNet != _networks.end(); ++itNet){
		// first  = index as int
		int index = (*itNet).first;
		// second = network pointer
		Network * network = (*itNet).second;

		NetworkMemento * networkMemento = memento->getNetworkMemento(index);
		MemoryOneModeNetwork * net =
				dynamic_cast<MemoryOneModeNetwork*>(network);
		net->restoreFromMemento(networkMemento);
	}

	// restore attribute containers
	std::map<int, AttributeContainer*>::iterator itAttCont = _attributeContainers.begin();
	for (; itAttCont != _attributeContainers.end(); ++itAttCont){
		AttributeContainerMemento * attMemento =
				memento->getAttributeContainerMemento((*itAttCont).first);
		(*itAttCont).second->restoreFromMemento(attMemento);
	}

	// restore global attributes
	std::map<int, double>::iterator itGlobAtt = _globalAttributes.begin();
	for (; itGlobAtt != _globalAttributes.end(); ++itGlobAtt){
		_globalAttributes[(*itGlobAtt).first] = (*itGlobAtt).second;
	}

}

void ProcessStateMemento::setNetworkMemento(int i, NetworkMemento* memento) {
	_networkMementos[i] = memento;
}

void ProcessStateMemento::setGlobalAttribute(int i, double value) {
	_globalAttributes[i] = value;
}

void ProcessStateMemento::setAttributeContainerMemento(int i,
		AttributeContainerMemento* memento) {
	_attributeContainerMementos[i] = memento;
}

NetworkMemento* ProcessStateMemento::getNetworkMemento(int i) {
	return _networkMementos[i];
}

AttributeContainerMemento* ProcessStateMemento::getAttributeContainerMemento(
		int i) {
	return _attributeContainerMementos[i];
}

double ProcessStateMemento::getGlobalAttribute(int i) {
	return _globalAttributes[i];
}
