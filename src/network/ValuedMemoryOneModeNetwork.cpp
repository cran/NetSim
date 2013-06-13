/*
 * ValuedMemoryOneModeNetwork.cpp
 *
 *  Created on: Jan 28, 2013
 *      Author: cws
 */

#include "ValuedMemoryOneModeNetwork.h"

ValuedMemoryOneModeNetwork::ValuedMemoryOneModeNetwork(
		int size, bool directed, bool reflexive) :
		MemoryOneModeNetwork(size, directed, reflexive){

	init();

}

ValuedMemoryOneModeNetwork::ValuedMemoryOneModeNetwork(
		std::vector<std::vector<double> > graph,
		bool directed, bool reflexive) :
			MemoryOneModeNetwork(graph, directed, reflexive){

	init();
}

// TODO: should be implemented in O(log(m)), m = number of ties
// assuming that the key set is ordered
double ValuedMemoryOneModeNetwork::getMaxTieValueBelow(double threshold) {
	std::map<double, std::set<std::pair<int, int> > >::iterator it;
	double maxValue = 0.0;
	for (it = _valueTiesMap.begin(); it != _valueTiesMap.end(); ++it){
		if ( (*it).first >= threshold) return maxValue;
		maxValue = (*it).first;
	}
	return threshold;
}

// TODO: should be implemented in O(log(m)), m = number of ties
// assuming that the key set is ordered
double ValuedMemoryOneModeNetwork::getMinTieValueAbove(double threshold) {
	std::map<double, std::set<std::pair<int, int> > >::iterator it;
	for (it = _valueTiesMap.begin(); it != _valueTiesMap.end(); ++it){
		if ( (*it).first > threshold) return (*it).first;
	}
	return threshold;
}

// TODO: inefficient implementation
// It would be better to have a internal list representation of positive values
// It should be ordered by tie value so that requests like: "What is the minimum
// tie value > threshold?" can be answered quickly
void ValuedMemoryOneModeNetwork::addToTieValues(double value) {
/*
	int i = 0;
	std::vector<std::vector<double> >::iterator itRow = _graph.begin();
	for(; itRow != _graph.end(); ++itRow){
		int j = 0;
		std::vector<double>::iterator itCol = (*itRow).begin();
		for (; itCol != (*itRow).end(); ++itCol){
			double oldValue = (*itCol);
			// TODO this must be a setTieValue call! (see multiply)
			setTie(i,j,oldValue + value);
			j++;
		}
		i++;
	}
*/
}

// TODO: very inefficient implementation O(n^2) instead of O(m)
// m number of ties, n number of nodes
void ValuedMemoryOneModeNetwork::multiplyTieValues(double value) {
/*
	int i = 0;
	std::vector<std::vector<double> >::iterator itRow = _graph.begin();
	for(; itRow != _graph.end(); ++itRow){
		int j = 0;
		std::vector<double>::iterator itCol = (*itRow).begin();
		for (; itCol != (*itRow).end(); ++itCol){
			double oldValue = (*itCol);
			// TODO this must be a setTieValue call! (see multiply)
			setTie(i,j,oldValue * value);
			j++;
		}
		i++;
	}
*/
}


void ValuedMemoryOneModeNetwork::init() {
	// basically does what updateInternalRepresentation() does
	// but without calling base class
	for (int i = 0; i < _size; i++){
		for (int j = 0; j < _size; j++){
			_tieValueMap[std::make_pair(i,j)] = getTieValue(i,j);
			( _valueTiesMap[getTieValue(i,j)] ).insert( std::make_pair(i,j) );
		}
	}

}

double ValuedMemoryOneModeNetwork::getTieValue(int i, int j) {
	//double check whether the pair is deleted automatically
	std::pair<int, int> pair = std::make_pair(i, j);
	return _tieValueMap[pair];

}

void ValuedMemoryOneModeNetwork::updateInternalRepresentation(int i, int j,
		double oldValue, double newValue) {

	MemoryOneModeNetwork::updateInternalRepresentation(i, j, oldValue, newValue);

	// update edge list representation
	_tieValueMap[std::make_pair(i,j)] = newValue; //automatically replaces old value
	( _valueTiesMap[newValue] ).insert( std::make_pair(i,j) );
	( _valueTiesMap[oldValue] ).erase( std::make_pair(i,j) );



}
