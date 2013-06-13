/*
 * Simulator.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#include "Simulator.h"

Simulator::Simulator(ProcessState * processState, ModelManager * modelManager,
		double periodLength) {

	// TODO:check that periodLength > 0

	_time = 0;
	_iterationSteps = 0;
	_periodLength = periodLength;
	_processState = processState;
	_modelManager = modelManager;
	_verbose = true;
	_debug = false;
}

void Simulator::simulate() {

	if(_verbose){
		Output() << "Starting simulation. Length = " << _periodLength << ", Time = " << _time << "\n";
	}
	clock_t timeStart = clock();

	double nextPercentageToPrint = 0;
	_iterationSteps = 0;

	while( _time < _periodLength){

		//simple progress bar
		if (nextPercentageToPrint <= (_time / _periodLength)){
			if (nextPercentageToPrint == 0 && _verbose){
				Output() << "Progress (in 10%) [";
			}

			if (_verbose & !_debug){
				// std::cout << "=" << std::flush;
				Output() << "=";
				Output().flush();
			}
			if (_debug){
				Output().endl();
				Output() << nextPercentageToPrint *100 << " % reached.";
				Output().endl();
			}

			nextPercentageToPrint += 0.1;
		}


		// TIME MODELS
		std::vector<TimeModel*> * timeModels = _modelManager->getTimeModels();
		_nextTimeModels.clear();
		// maximum time span
		double timeSpan = (_periodLength - _time);
		if (_debug){
			Output() << "Remaining time = " << timeSpan;
			Output().endl();
		}

		for (std::vector<TimeModel*>::iterator it = timeModels->begin();
				it != timeModels->end(); ++it){
			double newTimeSpan = (*it)->getTimeSpan(_processState);
			if (newTimeSpan == timeSpan){
				_nextTimeModels.push_back(*it);
			}
			if (newTimeSpan < timeSpan){
				timeSpan = newTimeSpan;
				_nextTimeModels.clear();
				_nextTimeModels.push_back(*it);
			}
		}
		_time += timeSpan;

		if (_debug){
			Output() << _nextTimeModels.size() <<
					" TimeModel(s) chosen with timespan " << timeSpan;
			Output().endl();
		}

		// TIME UPDATES
		std::vector<TimeUpdater *> * timeUpdaters = _modelManager->getTimeUpdater();
		for (std::vector<TimeUpdater *>::iterator it = timeUpdaters->begin();
				it != timeUpdaters->end(); ++it){
			(*it)->update(_processState, timeSpan);
		}
		if (_debug){
			Output() << "Time updates done.";
			Output().endl();
		}


		// CHANGE MODELS
		// get change models from time models
		_nextChangeModels.clear();
		for (std::vector<TimeModel*>::iterator it = _nextTimeModels.begin();
				it != _nextTimeModels.end(); ++it){
			_nextChangeModels.insert(_nextChangeModels.end(),
					_modelManager->getChangeModels(*it)->begin(),
					_modelManager->getChangeModels(*it)->end());
		}
		// get change results and create update vector
		_nextChangeResults.clear();
		_nextUpdaters.clear();
		for (std::vector<ChangeModel * >::iterator it = _nextChangeModels.begin();
				it != _nextChangeModels.end(); ++it){
			(*it)->setDebug(_debug);
			ModelResult * result = (*it)->getChange(_processState);
			_nextChangeResults.push_back(result);
			std::vector<Updater *> * updaters = _modelManager->getUpdaters(*it);
			_nextUpdaters.push_back(updaters);

		}

		if (_debug) {
			Output() << "Change models done.";
			Output().endl();
		}


		// UPDATES
		if (_nextChangeResults.size() != _nextUpdaters.size())
			return; // throw error
		for (size_t i = 0; i < _nextChangeResults.size(); i++){
			for (std::vector<Updater *>::iterator it = (*_nextUpdaters[i]).begin();
					it != (*_nextUpdaters[i]).end(); ++it){

				if (_debug) (*it)->setDebug();
				(*it)->update(_processState,
						_nextChangeResults[i]);
			}
		}

		if (_debug){
			Output() << "Change updates done.";
			Output().endl();
		}

		// delete result pointers
		// TODO maybe replace with boost::ptr_vector
		for (std::vector<ModelResult * >::iterator it = _nextChangeResults.begin();
				it != _nextChangeResults.end(); ++it){
			delete (*it);
		}

		++_iterationSteps;
	} // while loop

	if (_verbose) {
		Output() << "] done.";
		Output().endl();
	}


	// report time and iteration steps
	if(_verbose){
		double duration = (double)(clock() - timeStart)/CLOCKS_PER_SEC;
		double durationRounded = ( round( duration * 100 ) ) / 100;
		double timePerIteration = duration / ((double) _iterationSteps);
		double timePerIterarionRounded = ( round( timePerIteration * 100 ) ) / 100;
		/* commented out as incompatible with CRAN
		printf("Iteration steps: %d\n", _iterationSteps);
		printf("Simulation time: %.2fs\n", duration);
		printf("Time per iteration: %.6fs\n", duration / ((double) _iterationSteps));
		*/
		Output() << "Iteration steps: " << _iterationSteps << "\n";
		Output() << "Simulation time: " << durationRounded << "s\n";
		Output() << "Time per iteration: " << timePerIterarionRounded << "s\n";
	}

	// reset simulator
	_time = 0;

}

SimulationResult Simulator::getSimulationResult() {
	return _result;
}

void Simulator::nextIterationStep() {
	//std::string messageIterationStep = "";


}

bool Simulator::isTimeLeft() {
	return (_time < _periodLength);
}

int Simulator::getIterationSteps() {
	return _iterationSteps;
}

void Simulator::setVerbose(bool verbose) {
	_verbose = verbose;
}

double Simulator::getTimeUntilPeriodEnd() {
	return (_periodLength - _time);
}

std::pair<double, std::set<TimeModel *> > Simulator::chooseTimeModels(
		std::vector<TimeModel*> * timeModels,
		double remainingSimulationTime) {

	std::set<TimeModel *> nextTimeModels;


	for (std::vector<TimeModel*>::iterator itTimeModel = timeModels->begin();
			itTimeModel != timeModels->end();
			++itTimeModel){

		double latestTimeSpan = (**itTimeModel).getTimeSpan(_processState);

		// several changes at the same time
		if (latestTimeSpan == remainingSimulationTime){
			nextTimeModels.insert(*itTimeModel);
		}

		// a new shorter time interval
		if (latestTimeSpan < remainingSimulationTime){
			remainingSimulationTime = latestTimeSpan;
			nextTimeModels.clear();
			nextTimeModels.insert(*itTimeModel);
		}

	}

	return std::make_pair(remainingSimulationTime, nextTimeModels);

}

void Simulator::applyTimeUpdates(std::vector<TimeUpdater*> * timeUpdaters,
		double timeSpan) {
	if (timeUpdaters->empty()) return;
	else {

		for (std::vector<TimeUpdater*>::iterator it = timeUpdaters->begin();
				it != timeUpdaters->end(); ++it){

			(*it)->update(_processState,
					timeSpan);
		}

	}

}

std::vector< std::pair<ModelResult*, Updater*> > Simulator::applyChangeModels(
		std::vector<ChangeModel *> * changeModels) {

	// run CHANGE MODELS of each TimeModel and collect results; link these to updates
	std::vector< std::pair<ModelResult*, Updater*> > resultUpdaterPairs;
	std::vector<ChangeModel*>::iterator itChangeModels = changeModels->begin();

	for (; itChangeModels != changeModels->end(); ++itChangeModels){
			ModelResult * result =  (*itChangeModels)->getChange(_processState);
			std::vector<Updater*> * updaters =
					_modelManager->getUpdaters(*itChangeModels);
			// link all Updaters to results
			std::vector<Updater*>::iterator itUpdaters = updaters->begin();
			for (; itUpdaters != updaters->end(); ++itUpdaters){
				resultUpdaterPairs.push_back(std::make_pair(result, *itUpdaters));
			} // updaters

	} // changeModels

	return resultUpdaterPairs;
}

void Simulator::setDebug(bool debug) {
	_debug = debug;
}

void Simulator::applyChangeUpdates(
	std::vector<std::pair<ModelResult*, Updater*> > resultUpdaterPairs) {
	// This part is separate as updates are subsequently applied to the previous process state
	std::vector< std::pair<ModelResult*, Updater*> >::iterator itResultUpdaterPairs =
			resultUpdaterPairs.begin();
	for (; itResultUpdaterPairs != resultUpdaterPairs.end(); ++itResultUpdaterPairs){
		Updater * updater = (*itResultUpdaterPairs).second;
		ModelResult * result = (*itResultUpdaterPairs).first;
		updater->update(_processState, result);
	}

}
