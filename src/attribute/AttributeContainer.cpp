/*
 * AttributeContainer.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: cws
 */

#include "AttributeContainer.h"


AttributeContainer::AttributeContainer(std::vector<double> values) {
	init(values, - DBL_MAX, DBL_MAX, 0, 0.0);
}
AttributeContainer::AttributeContainer(int n, double value) {
	std::vector<double> v(n, value);
	init(v, - DBL_MAX, DBL_MAX, 0, 0.0);
}

/*
AttributeContainer::AttributeContainer(std::vector<double> values,
		double min, double max, double by, double defaultValue) {
	init(values, min, max, by, defaultValue);

}

AttributeContainer::AttributeContainer(int n, double value, double min,
		double max, double by, double defaultValue) {
	std::vector<double> v(n, value);
	init(v, min, max, by, defaultValue);
}
*/

AttributeContainer::~AttributeContainer() {
	// delete _actorValueMap;
}


bool AttributeContainer::increaseValue(int i) {
	return setValue(i, _actorValueMap[i] + _by);
}


bool AttributeContainer::decreaseValue(int i) {
	return setValue(i, _actorValueMap[i] - _by);
}


bool AttributeContainer::setValue(int i, double value) {
	bool r = false;

	// check that value is a multiple of the by attribute + the min value
	double checkedValue = value;
	if (_by >0 )
		checkedValue = (round((value - _min) / _by) * _by) + _min;

	if (value != checkedValue)
		Output() << "Incorrect attribute value: " << value << "\n";

	if ( (value >= _min) & (value <= _max) ){
		_actorValueMap[i] = value;
		r = true;
	}

	return r;
}


double AttributeContainer::getValue(int i) {
	return _actorValueMap[i];
}


bool AttributeContainer::isMax(int i) {
	return ( _actorValueMap[i] == _max);
}


bool AttributeContainer::isMin(int i) {
	return ( _actorValueMap[i] == _min);
}


int AttributeContainer::getSize() {
	return _actorValueMap.size();
}

bool AttributeContainer::setAsMax(int i) {
	return setValue(i, _max);
}

bool AttributeContainer::setAsMin(int i) {
	return setValue(i, _min);
}

double AttributeContainer::getMin() {
	return _min;
}

double AttributeContainer::getMax() {
	return _max;
}

double AttributeContainer::getBy() {
	return _by;
}

AttributeContainerMemento* AttributeContainer::saveToMemento() {
	AttributeContainerMemento * memento = new AttributeContainerMemento();
	memento->setActorValueMap(_actorValueMap);
	return memento;
}

void AttributeContainer::restoreFromMemento(
		AttributeContainerMemento* memento) {
	_actorValueMap = memento->getActorValueMap();
}

double AttributeContainer::getDefaultValue() {
	return _defaultValue;
}

void AttributeContainer::setDefaultValue(double defaultValue) {
	_defaultValue = defaultValue;
}

void AttributeContainer::addActor(size_t id, double value) {
	if (_actorValueMap.find(id) != _actorValueMap.end())
		throw std::invalid_argument(
				"ID already exists. (inclusion in attribute container)");
	_actorValueMap[id] = value;
}

void AttributeContainer::addActor(size_t id) {
	return addActor(id, _defaultValue);
}

void AttributeContainer::deleteActor(size_t id) {
	_actorValueMap.erase(id);
}

void AttributeContainer::setMin(double min) {
	_min = min;
}

void AttributeContainer::setMax(double max) {
	_max = max;
}

void AttributeContainer::setBy(double by) {
	_by = by;
}

void AttributeContainer::init(std::vector<double> values, double min,
	double max, double by, double defaultValue) {
	// check validity of values
	if (by < 0 )
		throw std::invalid_argument("'by' argument cannot be < 0");
	if (max <= min)
		throw std::invalid_argument("Maximum value is not greater than minimum value");
	if ( (max - min) < by)
		throw std::invalid_argument("'by' argument is greater than max - min value");
	if ( (by > 0) & (fmod( (max - min), by ) != 0.0) )
		throw std::invalid_argument("'max - min' is not a multiple of 'by' argument");


	// initialize variables
	_min = min;
	_max = max;
	_by = by;
	_defaultValue = defaultValue;
	for (size_t i = 0; i < values.size(); i++){
		if ( (values[i] < _min) | (values[i] > _max) ){
			throw std::invalid_argument("initial value out of range");
		}
		setValue(i, values[i]);
	}

}

void AttributeContainerMemento::setActorValueMap(
		std::map<int, double> actorValueMap) {
	_actorValueMap = actorValueMap;
}

std::map<int, double> AttributeContainerMemento::getActorValueMap() {
	return _actorValueMap;
}

ScaleAttributeContainer::ScaleAttributeContainer(std::vector<double> values,
		double min, double max, double by) : AttributeContainer(values) {
	double defaultValue = min;
	init(values, min, max, by, defaultValue);
}

ScaleAttributeContainer::ScaleAttributeContainer(int n, double value,
		double min, double max, double by) : AttributeContainer(n, value){
	std::vector<double> v(n, value);
	double defaultValue = min;
	init(v, min, max, by, defaultValue);
}

