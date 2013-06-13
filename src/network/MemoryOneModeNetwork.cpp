/*
 * MemoryOneModeNetwork.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: cws
 */

#include "MemoryOneModeNetwork.h"

MemoryOneModeNetwork::MemoryOneModeNetwork(int size, bool directed, bool reflexive){
	init(size, directed, reflexive);
}

MemoryOneModeNetwork::MemoryOneModeNetwork(
		std::vector<std::vector<double> > graph, bool directed, bool reflexive){
	int size = graph.size();
	init(size, directed, reflexive);
	for (int i = 0; i < graph.size(); i++){
		for (int j = 0; j < graph[i].size(); j++){
			if (graph[i][j] != 0){
				setTie(i,j,graph[i][j]);
			}
		}
	}
}

MemoryOneModeNetwork::~MemoryOneModeNetwork() {
	typedef std::map<int,std::set<int>* >::iterator it_type;
	for (it_type it = _outgoingNeighborsMap.begin();
			it != _outgoingNeighborsMap.end(); ++it){
		delete (*it).second;
	}
	for (it_type it = _incomingNeighborsMap.begin();
			it != _incomingNeighborsMap.end(); ++it){
		delete (*it).second;
	}

}

int MemoryOneModeNetwork::getOutDegree(int i) {
	return _outDegreeMap[i];
}

int MemoryOneModeNetwork::getInDegree(int i) {
	return _inDegreeMap[i];
}

void MemoryOneModeNetwork::init(int size, bool directed, bool reflexive){

	_size = size; // updated by addActor function
	_directed = directed;
	_reflexive = reflexive;

	// initialize lookup maps
	for (int i = 0; i < size; i++){
		initActor(i);
	}

}

void MemoryOneModeNetwork::initActor(int id) {
	_actorIDs.insert(id);

	// addActor(i); // updates the size (_size)
	_inDegreeMap.insert(
			std::map<int,int>::value_type(id,0)
			);
	_outDegreeMap.insert(
			std::map<int,int>::value_type(id,0)
			);
	// initialize neighbor maps with empty sets
	_outgoingNeighborsMap.insert(
			std::map<int, std::set<int>* >::value_type(id,new std::set<int>())
			);
	_incomingNeighborsMap.insert(
			std::map<int, std::set<int>* >::value_type(id,new std::set<int>())
			);

}

std::set<int> MemoryOneModeNetwork::getOutgoingNeighbors(int i) {
	return *_outgoingNeighborsMap[i];
}

std::set<int> MemoryOneModeNetwork::getIncomingNeighbors(int i) {
	return *_incomingNeighborsMap[i];
}

std::set<int> MemoryOneModeNetwork::getReciprocalNeighbors(int i) {
	return intersectSets(_incomingNeighborsMap[i], _outgoingNeighborsMap[i]);
}


std::set<int> MemoryOneModeNetwork::getNodesInDistanceTwo(int i) {
	std::set<int> distanceTwoNeighbors;

	for (std::set<int>::iterator it = _outgoingNeighborsMap[i]->begin(); it != _outgoingNeighborsMap[i]->end(); ++it){
		// insert neighbor sets of neighbors
		distanceTwoNeighbors.insert(
				_outgoingNeighborsMap[(*it)]->begin(), _outgoingNeighborsMap[(*it)]->end()
				);
	}

	distanceTwoNeighbors.erase(i);
	return distanceTwoNeighbors;

}

MemoryOneModeNetworkMemento * MemoryOneModeNetwork::saveToMemento() {
	MemoryOneModeNetworkMemento * memento = new MemoryOneModeNetworkMemento;
	memento->saveInDegreeMap(_inDegreeMap);
	memento->saveOutDegreeMap(_outDegreeMap);
	memento->saveIncomingNeighborsMap(_incomingNeighborsMap);
	memento->saveOutgoingNeighborsMap(_outgoingNeighborsMap);
	return memento;
}

void MemoryOneModeNetwork::restoreFromMemento(NetworkMemento * memento) {

	MemoryOneModeNetworkMemento* myMemento =
			dynamic_cast<MemoryOneModeNetworkMemento*>(memento);
	_inDegreeMap = myMemento->getSavedInDegreeMap();
	_outDegreeMap = myMemento->getSavedOutDegreeMap();
	_incomingNeighborsMap = myMemento->getSavedIncomingNeighborsMap(
			_incomingNeighborsMap);
	_outgoingNeighborsMap = myMemento->getSavedOutgoingNeighborsMap(
			_outgoingNeighborsMap);
}

bool MemoryOneModeNetwork::hasTie(int i, int j) {
	if (_outgoingNeighborsMap[i]->find(j) != _outgoingNeighborsMap[i]->end())
		return true;
	else return false;
}

double MemoryOneModeNetwork::getTieValue(int i, int j) {
	if (_outgoingNeighborsMap[i]->find(j) != _outgoingNeighborsMap[i]->end())
		return 1.0;
	else return 0.0;

}

// quite costly to be called
std::vector<std::vector<double> > MemoryOneModeNetwork::getGraph() {
	std::vector<std::vector<double> > graph(_size, std::vector<double>(_size));
	for (int i = 0; i < _size; i++){
		for (int j = 0; j < _size; j++){
			graph[i][j] = getTieValue(i, j);
		}
	}
	return graph;

}

bool MemoryOneModeNetwork::addTie(int i, int j) {
	return setTie(i, j, 1.0);
}

bool MemoryOneModeNetwork::setTie(int i, int j, double newValue) {
	// check validity of index
	// if (!isIndexValid(i)) return false;
	// if (!isIndexValid(j)) return false;

	double oldValue = getTieValue(i, j);

	// no change at all
	if (oldValue == newValue) return true;

	// trying to set a reflexive tie in a non-reflexive network
	if((!_reflexive) && (i == j)) return false;

	// Sets tie value and updates lookup maps
	updateInternalRepresentation(i, j, oldValue, newValue);
	if (!_directed) {
		double oldValue = getTieValue(j,i);
		updateInternalRepresentation(j, i, oldValue, newValue);
	}


	return true;

}

bool MemoryOneModeNetwork::removeTie(int i, int j) {
	return setTie(i, j, 0.0);
}

int MemoryOneModeNetwork::getSize() {
	return _size;
}

bool MemoryOneModeNetwork::isDirected() const {
	return _directed;
}

bool MemoryOneModeNetwork::isReflexive() const {
	return _reflexive;
}

bool MemoryOneModeNetwork::isIndexValid(int i) {
	return((i >= 0) & (i < _size));
}

void MemoryOneModeNetwork::addActor(size_t id) {
	initActor(id);
	_size++;
}

void MemoryOneModeNetwork::deleteActor(size_t id) {
	std::set<int>::iterator itNeighbor = _outgoingNeighborsMap[id]->begin();
	for (; itNeighbor != _outgoingNeighborsMap[id]->end(); ++itNeighbor){
		int j = (*itNeighbor);
		_incomingNeighborsMap[j]->erase(id);
		_inDegreeMap[j]--;
	}


	itNeighbor = _incomingNeighborsMap[id]->begin();
	for (; itNeighbor != _incomingNeighborsMap[id]->end(); ++itNeighbor){
		int j = (*itNeighbor);
		_outgoingNeighborsMap[j]->erase(id);
		_outDegreeMap[j]--;
	}

	delete _outgoingNeighborsMap[id];
	delete _incomingNeighborsMap[id];
	_outgoingNeighborsMap.erase(id);
	_incomingNeighborsMap.erase(id);

	_actorIDs.erase(id);

	_size--;

}

const std::set<int> MemoryOneModeNetwork::getActorIDs() {
	return _actorIDs;
}

std::set<int> MemoryOneModeNetwork::intersectSets(std::set<int>* set1,
		std::set<int> * set2) {
	std::set<int> resultSet;

	std::set_intersection(set1->begin(), set1->end(), set2->begin(), set2->end(),
			std::inserter(resultSet, resultSet.end()));

	 return resultSet;
}

std::set<int> MemoryOneModeNetwork::getIntermediateNodesBackward(int i, int j) {
	return intersectSets(_incomingNeighborsMap[i], _outgoingNeighborsMap[j]);
}

std::set<int> MemoryOneModeNetwork::getIntermediateNodesUp(int i, int j) {
	return intersectSets(_outgoingNeighborsMap[i], _outgoingNeighborsMap[j]);
}

std::set<int> MemoryOneModeNetwork::getIntermediateNodesDown(int i, int j) {
	return intersectSets(_incomingNeighborsMap[i], _incomingNeighborsMap[j]);
}

std::set<int> MemoryOneModeNetwork::getIntermediateNodesForward(int i, int j) {
	return intersectSets(_outgoingNeighborsMap[i], _incomingNeighborsMap[j]);
}

void MemoryOneModeNetwork::updateInternalRepresentation(int i, int j,
		double oldValue, double newValue) {
	// load function in super class first
	// OneModeNetwork::updateInternalRepresentation(i, j, oldValue, newValue);

	// ignore recursive ties
	if ( (!_reflexive) & (i == j) ) return;

	// Tie inclusion
	if (newValue > 0) {
		if (oldValue == 0 ){
			_inDegreeMap[j]++;
			_outDegreeMap[i]++;
			_outgoingNeighborsMap[i]->insert(j);
			_incomingNeighborsMap[j]->insert(i);
		}
	}

	// Tie removal
	if (newValue == 0) {
		if (oldValue > 0) {
			_inDegreeMap[j]--;
			_outDegreeMap[i]--;
			_outgoingNeighborsMap[i]->erase(j);
			_incomingNeighborsMap[j]->erase(i);
		}
	}

	// a change of a value is ignored in case
	// of this binary network implementation


}

MemoryOneModeNetworkMemento::MemoryOneModeNetworkMemento() {
}

std::map<int, int> MemoryOneModeNetworkMemento::getSavedInDegreeMap() {
	return _inDegreeMap;
}

std::map<int, int> MemoryOneModeNetworkMemento::getSavedOutDegreeMap() {
	return _outDegreeMap;
}

std::map<int, std::set<int> *> MemoryOneModeNetworkMemento::getSavedIncomingNeighborsMap(
		std::map<int, std::set<int>* > source) {

	return copyNeighborsSet(source, _incomingNeighborsMap);
}

std::map<int, std::set<int> *> MemoryOneModeNetworkMemento::getSavedOutgoingNeighborsMap(
		std::map<int, std::set<int>* > source) {
	return copyNeighborsSet(source, _outgoingNeighborsMap);
}

void MemoryOneModeNetworkMemento::saveInDegreeMap(
		const std::map<int, int> inDegreeMap) {
	_inDegreeMap = inDegreeMap;
}

void MemoryOneModeNetworkMemento::saveOutDegreeMap(
		const std::map<int, int> outDegreeMap) {
	_outDegreeMap = outDegreeMap;
}

void MemoryOneModeNetworkMemento::saveIncomingNeighborsMap(
		const std::map<int, std::set<int> *> incomingNeighborsMap) {

	for (std::map<int, std::set<int> *>::const_iterator it = incomingNeighborsMap.begin();
			it != incomingNeighborsMap.end();
			++it){
		// TODO this does not free the linked sets!
		delete _incomingNeighborsMap[(*it).first];
		// copy set
		_incomingNeighborsMap[(*it).first] = new std::set<int>();

		std::set<int>::iterator itNeighbors = (*it).second->begin();
		for (; itNeighbors != (*it).second->end(); ++itNeighbors){
			_incomingNeighborsMap[(*it).first]->insert(*itNeighbors);
		}
		// std::set<int> * nodes = new std::set<int>(*((*it).second));
		// _incomingNeighborsMap[(*it).first] = nodes;

	}
}

void MemoryOneModeNetworkMemento::saveOutgoingNeighborsMap(
		const std::map<int, std::set<int> *> outgoingNeighborsMap) {
	for (std::map<int, std::set<int> *>::const_iterator it = outgoingNeighborsMap.begin();
			it != outgoingNeighborsMap.end(); ++it){
		// TODO this does not free the linked sets!
		delete _outgoingNeighborsMap[(*it).first];
		std::set<int> * nodes = new std::set<int>(*((*it).second));
		_outgoingNeighborsMap[(*it).first] = nodes;
	}
}

std::map<int, std::set<int> *> MemoryOneModeNetworkMemento::copyNeighborsSet(
		std::map<int, std::set<int> *> destination, std::map<int, std::set<int> *> source) {



	// assuming that source sets are never deleted in the network
	// (however, they can be empty)
	std::map<int, std::set<int> *>::iterator it = destination.begin();

	for (; it != destination.end(); ++it){
		int actorIndex = (*it).first;
		(*it).second->clear();
		std::set<int>::iterator itNeighbors =
				source[actorIndex]->begin();
		for (; itNeighbors != source[actorIndex]->end();
				++itNeighbors){
			(*it).second->insert(*itNeighbors);
		}
	}

	return destination;


}
