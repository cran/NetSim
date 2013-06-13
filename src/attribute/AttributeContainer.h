/*
 * AttributeContainer.h
 *
 *  Created on: Nov 26, 2012
 *      Author: cws
 */

#ifndef ATTRIBUTECONTAINER_H_
#define ATTRIBUTECONTAINER_H_

#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <math.h>
#include <float.h>
#include "../io/Output.h"

// forward declaration
class AttributeContainerMemento;

/**
 * Represents a container for valued attributes,
 * e.g., a value measured on a discrete scale
 */
class AttributeContainer{

public:
	AttributeContainer(std::vector<double> values);
	AttributeContainer(int n, double value);
	// AttributeContainer(std::vector<double> values,
	// 		double min, double max, double by, double defaultValue = 0.0);
	// AttributeContainer(int n, double value,
	// 			double min, double max, double by, double defaultValue = 0.0);

	virtual ~AttributeContainer();

	/**
	 * Increases the value by the standard size if possible given maximum value
	 */
	bool increaseValue(int i);

	/**
	 * Decreases the value by the standard size if possible given minimum value
	 */
	bool decreaseValue(int i);

	/**
	 * set attribute to max value
	 */
	bool setAsMax(int i);

	/**
	 * set attribute to minimum value
	 */
	bool setAsMin(int i);

	/**
	 *  sets the attribute value
	 *  checks if it is in the range [min, max], if not, false is returned
	 */
	bool setValue(int i, double value);

	/**
	 * Get the attribute value of actor i
	 */
	double getValue(int i);

	/**
	 * Returns if the attribute value of actor i equals the maximum
	 */
	bool isMax(int i);

	/**
	 * Returns if the attribute value of actor i equals the minimum
	 */
	bool isMin(int i);

	/**
	 * Get the number of actors
	 */
	int getSize();

	/**
	 * get minimum value
	 */
	double getMin();

	/**
	 * get maximum value
	 */
	double getMax();

	/**
	 * get interval size
	 */
	double getBy();

	/**
	 * get default value for new actor inclusion
	 */
	double getDefaultValue();

	/**
	 * set default value
	 */
	void setDefaultValue(double defaultValue);

	/**
	 * set minimum value if the container is a scale
	 */
	void setMin(double min);

	/**
	 * set maximum value if the container is a scale
	 */
	void setMax(double max);

	/**
	 * set by value for ordered scales
	 */
	void setBy(double by);

	/**
	 * add actor with a particular value
	 */
	void addActor(size_t id, double value);

	/**
	 * add actor with default value
	 */
	void addActor(size_t id);

	/**
	 * delete actor with given id
	 */
	void deleteActor(size_t id);

	/**
	 * save to Memento
	 */
	AttributeContainerMemento * saveToMemento();

	/**
	 * restore from Memento
	 */
	void restoreFromMemento(AttributeContainerMemento * memento);



protected:
	double _min;
	double _max;
	double _by;
	double _defaultValue;
	// TODO test additional utility of boost
	// boost::unordered_map<int, double> _actorValueMap;
	std::map<int, double> _actorValueMap;

	void init(std::vector<double> values,
			double min = 0.0, double max = 1.0,
			double by = 1.0, double defaultValue = 0.0);
};

class ScaleAttributeContainer : public AttributeContainer{
public:
	ScaleAttributeContainer(std::vector<double> values, double min, double max, double by);
	ScaleAttributeContainer(int n, double value, double min, double max, double by);

};

class AttributeContainerMemento{
private:

	void setActorValueMap(std::map<int, double> actorValueMap);
	std::map<int, double> getActorValueMap();

	std::map<int, double> _actorValueMap;

	friend class AttributeContainer;
};


#endif /* ATTRIBUTECONTAINER_H_ */
