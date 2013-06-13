/*
 * rwrapper_simulator.cpp
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#include "rwrapper_simulator.h"

SEXP create_simulator(SEXP processStateManager_, SEXP modelManager_,
		SEXP periodLength_, SEXP verbose_, SEXP debug_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	ProcessState * processState = processStateManager->getProcessState();
	ModelManager * modelManager =
			Rcpp::XPtr<ModelManager>(modelManager_);
	double periodLength = Rcpp::as<double>(periodLength_);
	bool verbose = Rcpp::as<bool>(verbose_);
	bool debug = Rcpp::as<bool>(debug_);

	Simulator * simulator = new Simulator(processState, modelManager, periodLength);
	simulator->setVerbose(verbose);
	simulator->setDebug(debug);

	return Rcpp::XPtr<Simulator>(simulator, false);

	END_RCPP
}

SEXP simulate(SEXP simulator_) {
	BEGIN_RCPP

	Simulator * simulator = Rcpp::XPtr<Simulator>(simulator_);

	simulator->simulate();

	SimulationResult result = simulator->getSimulationResult();

	// return Rcpp::XPtr<SimulationResult>(&result, true);
	return Rcpp::wrap(1);

	END_RCPP
}

SEXP get_iteration_steps(SEXP simulator_) {
	BEGIN_RCPP

	Simulator * simulator = Rcpp::XPtr<Simulator>(simulator_);
	int steps = simulator->getIterationSteps();

	return Rcpp::wrap(steps);

	END_RCPP
}
