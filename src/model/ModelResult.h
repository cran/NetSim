/*
 * ModelResult.h
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#ifndef MODELRESULT_H_
#define MODELRESULT_H_

#include <string>
#include <set>

/**
 * A ModelResult object saves result information about a time or change model.
 * This information can subsequently used by a logger or an updater.
 */
class ModelResult{

public:
	virtual ~ModelResult(){ }
	// double getDoubleResult(){ return -1;}; // TODO is there a nicer way to define an "abstract class" with return functions that differ by type?
	// int getIntResult(){ return -1;};
	// virtual std::string getResultString() = 0;

};

class ActorModelResult : public ModelResult{

public:
	ActorModelResult(int actorIndex);

	int getActorIndex();

private:
	int _actorIndex;


};

class ActorSetModelResult : public ModelResult{

public:
	ActorSetModelResult(std::set<int> actorSet);

	std::set<int> getActorSet();

private:
	std::set<int> _actorSet;


};

class TwoTiesModelResult : public ModelResult{

public:
	TwoTiesModelResult(int actorIndex1,
			int actorIndex2,
			int actorIndex3,
			int actorIndex4);

	int getActorIndex1();
	int getActorIndex2();
	int getActorIndex3();
	int getActorIndex4();

private:
	int _actorIndex1;
	int _actorIndex2;
	int _actorIndex3;
	int _actorIndex4;

};


class TieModelResult : public ModelResult{
public:
	TieModelResult(int actorIndex1, int actorIndex2);

	TieModelResult& operator=( const TieModelResult& tmr );

	int getActorIndex1();

	int getActorIndex2();

private:
	int _actorIndex1;
	int _actorIndex2;


};

class ValueModelResult : public ModelResult{

public:
	ValueModelResult(double value);

	double getValue();

private:
	double _value;
};

class TimeModelResult : public ModelResult{

public:
	TimeModelResult(double timeSpan);

	double getDoubleResult();

	std::string getResultString();


private:
	double _timeSpan;
};

// TODO: implement ActorValueModelResult
// TODO: implement ActorSetModelResult

#endif /* MODELRESULT_H_ */
