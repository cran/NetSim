\name{effect_container}
\alias{create_effect_container}
\alias{add_to_effect_container}
\alias{create_effect}
\alias{create_effect.OneModeNetworkEffect}
\alias{create_effect.AttributeOneModeNetworkEffect}
\alias{create_effect.SimilarityAttributeOneModeNetworkEffect}
\alias{create_effect.AttributeEffect}
\alias{create_effect.MultiplexNetworkEffect}
\alias{add_effect}
\alias{add_effect.AttributeMultinomialChoiceNetworkChangeModel}
\alias{get_effect_type}
\docType{package}
\title{
	Overview of SAOM-related functions
}
\description{
	Overview of functions to specify stochastic actor-oriented models. The naming of
	effects is in line with the RSiena manual (see reference below). 
	
		The following effects are implemented in SAOMs for NetSim so far (to be extended, see RSiena manual for detailed explanations):\cr
	\emph{OneModeNetworkEffect} \cr
	- density \cr
	- recip \cr
	- transTrip \cr
	- cycle3 \cr
	- nbrDist2 \cr
	- inPop \cr
	- outPop \cr
	\emph{AttributeOneModeNetworkEffect} \cr
	- altX \cr
	- egoX \cr
	\emph{SimilarityAttributeOneModeNetworkEffect} \cr
	- simX \cr
	- totSim \cr
	\emph{AttributeEffect} \cr
	- linear \cr
	- quad \cr
	\emph{MultiplexNetworkEffect}
	- crprod	
}
\usage{
	create_effect_container()
	add_to_effect_container(effectContainer, effect, parameter)
	create_effect(name, ...)
	\method{create_effect}{OneModeNetworkEffect}(name, networkIndex, ...)
	\method{create_effect}{AttributeOneModeNetworkEffect}(name, attributeIndex, networkIndex, ...)
	\method{create_effect}{SimilarityAttributeOneModeNetworkEffect}(name, attributeIndex, networkIndex, meanSimilarityScore, ...)
	\method{create_effect}{AttributeEffect}(name, attributeIndex, ...)
	\method{create_effect}{MultiplexNetworkEffect}(name, networkIndex1, networkIndex2, ...)
	\method{add_effect}{AttributeMultinomialChoiceNetworkChangeModel}(changeModel, effect, attributeIndex, ...)
	get_effect_type(name)
}
\arguments{
	\item{effectContainer}{An effect container object}
	\item{effect}{An effect object created with create_effects function}
	\item{parameter}{Parameter of the effect specified}
	\item{name}{String name of the effect according to the SIENA manual}
	\item{changeModel}{A SAOM change model}
	\item{...}{Additional parameters depending on the type of the effect (see description above)}
	\item{attributeIndex}{Index of an attribute containing the (changeable) parameter}
	\item{meanSimilarityScore}{mean similarity score according to effect definitions in the SIENA manual}
	\item{networkIndex}{Index of a network}
	\item{networkIndex1}{Index of a network}
	\item{networkIndex2}{Index of a network}
}
\seealso{
	\code{\link{create_multinomial_choice_network_change_model}}
	\code{\link{create_multinomial_choice_behavior_change_model}}
	\code{\link{create_attribute_multinomial_choice_network_change_model}}
	\code{\link{create_tie_swap_updater}}
}
\references{
Ripley, R. M.; Snijders, T. A. B. & Preciado Lopez, P.: Manual for SIENA 4.0. (2012), 
Oxford: University of Oxford, Department of Statistics; Nuffield College. 
URL: http://www.stats.ox.ac.uk/~snijders/siena/RSiena_Manual.pdf
}
\examples{
## See sections 4.1, 4.2 and 4.5 in the package vignette for detailed examples
## Small example:

}