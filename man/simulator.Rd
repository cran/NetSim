\name{simulator}
\alias{create_simulator}
\alias{simulate}
\alias{get_iteration_steps}
\docType{package}
\title{
	Functions related to run a simulation 
}
\description{
	Functions related to run a simulation based on a process state and a model manager.
	A simulator object needs to be created, then the simulator can be run.
	After the simulation the number of simulation steps can be returned.
}
\usage{
	create_simulator(processState, modelManager, periodLength = 1, 
				verbose = TRUE, debug = FALSE)
	simulate(simulator)
	get_iteration_steps(simulator)
}
\arguments{
	\item{processState}{A process state object}
	\item{modelManager}{A model manager object}
	\item{periodLength}{A time span to simulate. The default is 1.}
	\item{verbose}{If TRUE it generates some basic information output including a progress bar}
	\item{debug}{If TRUE it generate a very detailed output for some models. Slows down the simulation process significantly.}
	\item{simulator}{A simulator object}
}
\seealso{
	\code{\link{create_process_state}}
	\code{\link{create_model_manager}}
}
\examples{
	## Detailed examples of process states and model managers can be found in 
	## section 4 of the package vignette.
	processState <- create_process_state()
	modelManager <- create_model_manager()
	
	processState <- create_process_state()
	processState <- add_global_attribute(processState, 0.0, name = "timer")
	timerIndex <- get_global_attribute_index(processState, name = "timer")
	myTimeModel <- create_round_based_time_model(timerIndex)
	modelManager <- add_time_model(modelManager, myTimeModel)
	
	timeSpan <- 20
	simulator <- create_simulator(processState, modelManager, timeSpan,
	verbose = TRUE, debug = FALSE)
	simulate(simulator)
}