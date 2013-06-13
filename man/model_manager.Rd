\name{model_manager}
\alias{create_model_manager}
\alias{add_time_model}
\alias{add_time_updater}
\alias{add_change_model}
\alias{add_updater}
\docType{package}
\title{
	Manage models using a model manager object
}
\description{
	A set of functions to create a model manager and to add 1. time models, 
	2. time updater, 3. change models and 4. updaters to it. 
	
	In general, model managers consist of a set of model chains that follow the
	generic four-step scheme sketched above. 
}
\usage{
	create_model_manager()
	add_time_model(modelManager, timeModel)
	add_time_updater(modelManager, timeUpdater)
	add_change_model(modelManager, timeModel, changeModel)
	add_updater(modelManager, changeModel, updater)	
}
\arguments{
	\item{modelManager}{A model manager object}
	\item{timeModel}{A time model object. See time model overview for examples.}
	\item{timeUpdater}{A time updater object. See time updater overview for examples.}
	\item{changeModel}{A change model object. See change model overview for examples.}
	\item{updater}{An updater. See updater overview for examples.}
}
\seealso{
	\code{\link{create_network}}
	\code{\link{create_attribute_container}}
}
\examples{
# minimal code example model manager
modelManager <- create_model_manager()

processState <- create_process_state()
processState <- add_global_attribute(processState, 0.0, name = "timer")
timerIndex <- get_global_attribute_index(processState, name = "timer")

myTimeModel <- create_round_based_time_model(timerIndex)

modelManager <- add_time_model(modelManager, myTimeModel)

simulator <- create_simulator(processState, modelManager, 20)
simulate(simulator)

}