/*
 * Simulator.h
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "../processstate/ProcessState.h"
#include "../model/ModelManager.h"
#include "../model/Updater.h"
#include <utility>  // pair
#include <stdio.h>
#include <list>
#include "../io/Output.h"

/**
 * Logs all changes in the simulation process
 * as a list of node-related, relational or global events
 * and provides a human-readable form
 */
class SimulationResult{

public:
	SimulationResult(){ }

	void addLine(std::string line){
		std::string newLine = line.append("\n");
		_textualResult.append(newLine);
	}

private:
	std::string _textualResult;

};

/**
 * Manages a simulation run based on a
 * process state, a model manager and a
 * time period to be simulated. Provides a
 * SimulationResult object.
 */
class Simulator{

public:
	Simulator(ProcessState * processState, ModelManager * modelManager,
			double periodLength);

	void simulate();

	SimulationResult getSimulationResult();

	int getIterationSteps();

	void setVerbose(bool verbose = true);

	void setDebug(bool debug = true);

private:
	double _time;
	double _periodLength;
	int _iterationSteps;
	bool _verbose;
	bool _debug;
	SimulationResult _result;
	ProcessState* _processState;
	ModelManager* _modelManager;
	std::vector<TimeModel * > _nextTimeModels;
	std::vector<ChangeModel * > _nextChangeModels;
	std::vector<ModelResult * > _nextChangeResults;
	std::vector<std::vector<Updater * > * >_nextUpdaters; // one for each result

	void nextIterationStep();
	bool isTimeLeft();
	double getTimeUntilPeriodEnd();

	/*
	 * Deprecated simulation routine
	 */
	void simulateOLD();

	/**
	 * returns a pair with time until next change
	 * and a set of all timeModel pointers that returned that time span
	 * first: time span
	 * second: index set of time models
	 */
	std::pair<double, std::set<TimeModel *> > chooseTimeModels(
			std::vector<TimeModel*> * timeModels,
			double remainingSimulationTime);

	/**
	 * apply time related updates
	 */
	void applyTimeUpdates(std::vector<TimeUpdater*> *, double timeSpan);

	/**
	 * apply change models
	 */
	std::vector< std::pair<ModelResult*, Updater*> > applyChangeModels(
			std::vector<ChangeModel *> * changeModels);

	/**
	 * apply change updates
	 */
	void applyChangeUpdates(std::vector< std::pair<ModelResult*, Updater*> >);

};




#endif /* SIMULATOR_H_ */
