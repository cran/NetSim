\name{updater}
\alias{create_tie_swap_updater}
\alias{create_actor_attribute_set_updater}
\alias{create_add_actor_updater}
\alias{create_add_ties_from_newborn_actor_updater}
\alias{create_set_attribute_of_newborn_actor_updater}
\alias{create_rewire_tie_updater}
\docType{package}
\title{
	Overview of updaters implemented in NetSim
}
\description{
	Overview of updaters implemented in NetSim. 
	When linking an updateer to a change model one has to make sure that the results
	of the change model can be interpreted by the updater.
}
\usage{
	create_tie_swap_updater(networkIndex)
	create_actor_attribute_set_updater(attributeIndex, actorIndex)
	create_add_actor_updater()
	create_add_ties_from_newborn_actor_updater(networkIndex)
	create_set_attribute_of_newborn_actor_updater(attributeIndex, value)
	create_rewire_tie_updater(networkIndex)
}
\arguments{
	\item{networkIndex}{Index of the focal network}
	\item{attributeIndex}{Index of the focal attribute container}
	\item{actorIndex}{Index of the focal actor}
	\item{value}{Value of the attribute}
}
\seealso{
	\code{\link{create_model_manager}}
}
\examples{
	## Extensive examples can be found in section 4 of the package vignette
	## A more detailed description is given in section 3.3 of the package vignette
}