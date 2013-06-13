\name{time_models}
\alias{create_round_based_time_model}
\alias{create_poisson_model}
\alias{create_attribute_poisson_model}
\docType{package}
\title{
	Overview of time models implemented in NetSim
}
\description{
	Overview of different time models implemented in NetSim. 
	The first model is a deterministic round-based time model,
	the second is a Poisson model based on a fixed (unchangeable) parameter,
	the third is a Poisson model for which parameters can be stored in an attribute container (changeable).
}
\usage{
	create_round_based_time_model(timerIndex, intervalLength = 1.0, startTime = 0.0)
	create_poisson_model(param = 1)
	create_attribute_poisson_model(attributeIndex)
}
\arguments{
	\item{timerIndex}{A pointer to a global attribute in the process state representing the timer}
	\item{intervalLength}{interval length between two subsequent events triggered}
	\item{startTime}{First point in time when the timer index is triggered}
	\item{param}{Fixed Poisson parameter}
	\item{attributeIndex}{A pointer to an attribute container giving a vector of concurring Poisson parameters}
}
\seealso{
	\code{\link{create_model_manager}}
}
\examples{
	processState <- create_process_state()

	# add attribute container to process state	
	attribute <- create_attribute_container(rep(2.0, 5))
	processState <- add_attribute_container(processState, attribute, name = "poissonAttribute")
	attributeIndex <- get_attribute_container_index(processState, name = "poissonAttribute")
	
	# add timer variable to process state
	processState <- add_global_attribute(processState, name = "timer", value = 0)
	timerIndex <- get_global_attribute_index(processState, name = "timer")
	
	
	# create time models based on process state pointers
	timeModel1 <- create_round_based_time_model(timerIndex)
	timeModel2 <- create_poisson_model(3.0)
	timeModel3 <- create_attribute_poisson_model(attributeIndex)
	
	# a change of the attribute container changes time model 3
	set_value(attribute, 0, 0.5)
	
}