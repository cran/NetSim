\name{process_state}
\alias{create_process_state}
\alias{add_network}
\alias{get_network_index}
\alias{get_network}
\alias{add_attribute_container} 
\alias{get_attribute_container_index}
\alias{get_attribute_container}
\alias{add_global_attribute}
\alias{get_global_attribute_index}
\alias{get_global_attribute}
\alias{get_process_state_name}
\docType{package}
\title{
Manage the process state
}
\description{
A set of functions to manage the process state of the simulation Markov process.
The process state can be created and networks, attribute containers and global attributes be added.
The IDs of these objects can be requested and are necessary for some sub-sequent functions. 
}
\usage{
create_process_state(name = "default")
add_network(processState, network, name = "defaultNetwork")
get_network_index(processState, name = "defaultNetwork")
get_network(processState, name = "defaultNetwork")
add_attribute_container(processState, attributeContainer, name = "defaultAttribute")
get_attribute_container_index(processState, name = "defaultAttribute")
get_attribute_container(processState, name = "defaultAttribute")
add_global_attribute(processState, value = 0.0, name = "defaultGlobalAttribute")
get_global_attribute_index(processState, name = "defaultGlobalAttribute")
get_global_attribute(processState, name = "defaultGlobalAttribute")
get_process_state_name(processState)
}
\arguments{
	\item{processState}{A process state object}
	\item{network}{A network object}
 	\item{attributeContainer}{An attribute container object}
 	\item{name}{A string name of an object}
 	\item{value}{A value to initialize the global attribute}
}
\seealso{
	\code{\link{create_network}}, \code{\link{create_attribute_container}}
}
\examples{
# create an empty process state
processState <- create_process_state()

nActors <- 5

# create network object and add to process state
network <- create_network(matrix(1, nActors, nActors))
processState <- add_network(processState, network, name = "friendship")
get_network_index(processState, name = "friendship")

# create attribute container and add to process state
attribute1 <- create_attribute_container(c(rep(0, nActors\%/\%2), rep(1, nActors - nActors\%/\%2)))
attribute2 <- create_attribute_container(c(rep(0, nActors\%/\%2), rep(1, nActors - nActors\%/\%2)))
processState <- add_attribute_container(processState, attribute1, name = "gender")
processState <- add_attribute_container(processState, attribute2, name = "age")
as.numeric(attribute1)
get_attribute_container_index(processState, name="gender")
get_attribute_container_index(processState, name="age")

# add a global variable to the process state
processState <- add_global_attribute(processState, value = 1, name = "timer")
get_global_attribute(processState, name="timer")
get_global_attribute_index(processState, name="timer")
}