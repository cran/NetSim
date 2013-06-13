/*
 * TimeModel.h
 *
 *  Created on: Nov 28, 2012
 *      Author: cws
 */

#ifndef TIMEMODEL_H_
#define TIMEMODEL_H_

#include "../utils/Random.h"
#include "../processstate/ProcessState.h"

/**
 * Abstract class defining a Time model
 */
class TimeModel{

public:
	virtual ~TimeModel() { }

	/**
	 * get time span until next change. A timespan can never be zero or less.
	 */
	virtual double getTimeSpan(ProcessState * processState) = 0;


};

class PoissonTimeModel : public TimeModel{

public:
	PoissonTimeModel(double poissonParameter);

	double getTimeSpan(ProcessState * processState);

	double getParameter();

private:
	double _poissonParameter;

};

/**
 * This class is to be used if actors join the simulation or
 * change their behavior stochstically.
 * It needs to be linked to a change model also determining
 * the "active" actor and not just her choices.
 */
class JointAttributePoissonTimeModel : public TimeModel{
public:
	JointAttributePoissonTimeModel(size_t attributeContainerIndex);

	double getTimeSpan(ProcessState * processState);

private:
	size_t _attributeContainerIndex;

};

class RoundBasedTimeModel : public TimeModel{

public:
	RoundBasedTimeModel(size_t timerIndex, double intervalLength, double startTime = 0);

	double getTimeSpan(ProcessState * processState);

private:
	size_t _timerIndex;
	double _intervalLength;
	double _startTime;
};



#endif /* TIMEMODEL_H_ */
