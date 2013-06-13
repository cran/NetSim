\name{pkg-internal}
\alias{create_effect.UnknownType}
\alias{create_effect.character}
\alias{add_effect_with_parameter}
\alias{add_effect_with_index}
\alias{create_siena_model_manager}
\alias{add_effect.MultinomialChoiceNetworkChoiceChangeModel}
\docType{package}
\title{
	Internal functions
}
\description{
	Internal functions that are not supposed to be called by the user directly 
	or are functions that are in the process of being tested.
}
\usage{
	\method{create_effect}{character}(name, ...)
	\method{create_effect}{UnknownType}(name, ...)
	add_effect_with_parameter(saom, effect, parameter)
	add_effect_with_index(saom, effect, attributeIndex)
	create_siena_model_manager(poissonParameter, dependentNetworkIndex, 
		effectNames, effectInitParameters, effectParameters, nActors)
	\method{add_effect}{MultinomialChoiceNetworkChoiceChangeModel}(changeModel, ...)
}
\arguments{
	\item{name}{effect name}
	\item{saom}{SAOM object}
	\item{effect}{effect object}
	\item{parameter}{effect parameter}
	\item{attributeIndex}{index to the attribute container capturing parameters}
	\item{poissonParameter}{homogenous Poisson parameter}
	\item{dependentNetworkIndex}{index of dependent network}
	\item{effectNames}{names of one mode effects as vector}
	\item{effectInitParameters}{initializing parameters}
	\item{effectParameters}{effect parameters}
	\item{nActors}{number of actors}
	\item{changeModel}{A change model}
	\item{...}{Additional arguments}
}
\keyword{internal}