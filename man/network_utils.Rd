\name{network_utils}
\alias{add_random_ties_to_network}
\alias{add_ring_lattice_to_network}
\docType{package}
\title{
	Utility functions for NetSim network objects
}
\description{
	Utility functions to quickly change macro features of NetSim network objects.
	The first function adds random ties to a network, the second function creates 
	a ring lattice structure to a network.
}
\usage{
	add_random_ties_to_network(network, probability = 0.5)
	add_ring_lattice_to_network(network, nReciprocalTies)
}
\arguments{
	\item{network}{A NetSim network object}
	\item{probability}{Bernoulli probability of a non-existing tie to be set to 1.}
	\item{nReciprocalTies}{The number of reciprocal ties one node has in the ring lattice structure.}
}
\seealso{
	\code{\link{create_network}}
}
\examples{
 nActors <- 5
 network <- create_network(matrix(0, nActors, nActors))
 
 add_random_ties_to_network(network, probability = 0.5)
 add_random_ties_to_network(network, probability = 1)
 
 # reset network to an empty network
 network <- create_network(matrix(0, nActors, nActors))
 add_ring_lattice_to_network(network, nReciprocalTies = 2)
 \dontrun{
 add_ring_lattice_to_network(network, nReciprocalTies = 0) # throws an error message
  }
}