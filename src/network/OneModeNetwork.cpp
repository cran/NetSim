/*
 * Network.cpp
 *
 *  Created on: Nov 20, 2012
 *      Author: cws
 */

#include "OneModeNetwork.h"

OneModeNetwork::OneModeNetwork(int size, bool directed, bool reflexive) {
	_size = size;
	int vectorLength = size * size;
	_graph = std::vector<double>(vectorLength, 0.0);
	_directed = directed;
	_reflexive = reflexive;
	init();

}

OneModeNetwork::OneModeNetwork(std::vector<std::vector<double> > graph,
		bool directed, bool reflexive) {
	_size = graph.size();
	_directed = directed;
	_reflexive = reflexive;
	_graph = stripGraph(graph, reflexive, directed);
	init();
}

OneModeNetwork::~OneModeNetwork() {
}

bool OneModeNetwork::addTie(int i, int j) {

	// check validity
	if (!isIndexesValid(i, j)) return false;

	// set tie value to default value of 1.0
	return setTie(i, j, 1.0);
}


bool OneModeNetwork::hasTie(int i, int j) {

	// check validity
	if (!isIndexesValid(i, j)) return false;

	return(getTieValue(i,j) > 0);

}

bool OneModeNetwork::removeTie(int i, int j) {

	// check validity
	if (!isIndexesValid(i, j)) return false;

	// set tie value
	return setTie(i, j, 0.0);

}

std::vector<std::vector<double> > OneModeNetwork::getGraph() {
	std::vector<std::vector<double> > graph(_size, std::vector<double>(_size));
	for (int i = 0; i < _size; i++){
		for (int j = 0; j < _size; j++){
			graph[i][j] = _graph[getInternalIndex(i, j)];
		}
	}
	return graph;
}

int OneModeNetwork::getSize(){
	return _size;
}

bool OneModeNetwork::setTie(int i, int j, double value) {
	// check validity of index
	if (!isIndexesValid(i, j)) return false;

	// no change at all
	if (getTieValue(i,j) == value) return true;

	// trying to set a reflexive tie in a non-reflexive network
	if((!_reflexive) && (i == j)) return false;

	// Sets tie value and updates lookup maps
	double oldValue = getTieValue(i, j);
	_graph[getInternalIndex(i, j)] = value;
	updateInternalRepresentation(i, j, oldValue, value);
	if (!_directed) {
		double oldValue = getTieValue(j,i);
		_graph[getInternalIndex(j,i)] = value;
		this->updateInternalRepresentation(j, i, oldValue, value);
	}


	return true;
}

bool OneModeNetwork::isIndexValid(int i) {
	return((i >= 0) & (i < _size));
}

bool OneModeNetwork::isIndexesValid(int i, int j) {
	return (isIndexValid(i) & isIndexValid(j));
}

void OneModeNetwork::updateInternalRepresentation(int i, int j,
		double oldValue, double newValue) {
	// no updates if the value did not change
	if (oldValue == newValue) return;

}

bool OneModeNetwork::isDirected() const {
	return _directed;
}

bool OneModeNetwork::isReflexive() const {
	return _reflexive;
}

double OneModeNetwork::getTieValue(int i, int j) {
	return _graph[getInternalIndex(i,j)];
}

//OneModeNetworkMemento* OneModeNetwork::saveToMemento() {
//}

void OneModeNetwork::restoreFromMemento(NetworkMemento* memento) {
}

void OneModeNetwork::init() {
}

int OneModeNetwork::getInternalIndex(int i, int j) {
	return (i * _size) + j;
}

std::vector<double> OneModeNetwork::stripGraph(
		std::vector<std::vector<double> > graph,
		bool reflexive,
		bool directed) {

	std::vector<double> graph1d(_size * _size, 0.0);

	// set reflexive ties to zero
	// TODO symmetrize
	for (size_t i = 0; i < graph.size(); i++){
		for (size_t j = 0; j < graph.size(); j++){
			graph1d[getInternalIndex(i, j)] = graph[i][j];
			if (!_reflexive){
				if (i == j){
					graph1d[getInternalIndex(i, j)] = 0;
				}
			}
		}
	}

	return graph1d;
}

void OneModeNetworkMemento::saveGraph(std::vector<double> graph) {
	_graph = graph; // copy constructor ?
}

std::vector<double> OneModeNetworkMemento::getGraph() {
	return _graph;
}
