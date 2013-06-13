\name{utils}
\alias{reset_random_seed}
\alias{get_random_seed}
\docType{package}
\title{
	General utility functions
}
\description{
	General utility functions to read and manipulate the random number generator 
	by setting and reading the random seed.
	This assures reproducibility of simulation results.
}
\usage{
	reset_random_seed(seed)
	get_random_seed()
}
\arguments{
	\item{seed}{Random number seed, a positive integer}
}
\examples{
	reset_random_seed(12)
	get_random_seed()
}