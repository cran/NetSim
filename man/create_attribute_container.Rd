\name{create_attribute_container}
\alias{create_attribute_container}
\alias{create_scale_attribute_container}
\alias{set_value}
\alias{attribute_container_as_list}
\alias{as.numeric.NetSimAttributeContainer}
\alias{as.double.NetSimAttributeContainer}
\alias{print.NetSimAttributeContainer}
\docType{package}
\title{
	Create an attribute container object and maintain it
}
\description{
	Create an attribute container object representing arbitrary values or
	values from a scale (factor variable) 
	based on a numeric vector, change single values of the container and 
	transform it back into a numeric vector. 
	
	A scale attribute container has to be chosen for individual \emph{dependent} variables 
	in stochastic actor-oriented models (SAOMs) for behavior change. A normal attribute
	container can be chosen to represent \emph{independent} individual variables in SAOMs.  
}
\usage{
	create_attribute_container(numericVector)	
	create_scale_attribute_container(numericVector, min = 0, max = 1, by = 1)
	set_value(attributeContainer, i, value)
	attribute_container_as_list(attributeContainer)
	\method{as.numeric}{NetSimAttributeContainer}(x, ...)
	\method{as.double}{NetSimAttributeContainer}(x, ...)
	\method{print}{NetSimAttributeContainer}(x, ...)
}
\arguments{
	\item{numericVector}{A numeric vector}
	\item{min}{Minimum value of the factor variable}
	\item{max}{Maximum value of the factor variable}
	\item{by}{Difference between the factor variables}
	\item{i}{Index of the attribute to be changed. Starts counting from 0, not 1.}
	\item{value}{Set attribute to this value}
	\item{attributeContainer}{A \pkg{NetSim} attribute container object}
	\item{x}{A \pkg{NetSim} attribute container object}
	\item{...}{Potential additional arguments}
}
\seealso{
	\code{\link{create_process_state}}, \code{\link{create_network}}
}
\examples{
nActors <- 5
numericVector <- c(rep(0, nActors\%/\%2), rep(1, nActors - nActors\%/\%2))
attributeContainer1 <- create_attribute_container(numericVector)
attributeContainer2 <- 
	create_scale_attribute_container(numericVector, min = 0, max = 2, by = 1)

attributeContainer1 <- set_value(attributeContainer1, i = 0, value = 5)
# does not change anything as 5 is out of range
attributeContainer2 <- set_value(attributeContainer2, i = 0, value = 5)

as.numeric(attributeContainer1)
as.numeric(attributeContainer2)
}