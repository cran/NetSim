\name{change_models}
\alias{create_multinomial_choice_network_change_model}
\alias{create_multinomial_choice_behavior_change_model}
\alias{create_jackson_rogers_change_model}
\alias{create_watts_strogatz_change_model}
\alias{create_attribute_multinomial_choice_network_change_model}
\docType{package}
\title{
	Overview of change models implemented in NetSim
}
\description{
	Overview of change models implemented in NetSim. The first two and the fifth are 
	define stochastic actor-oriented models for network or behavior change. The third
	defines a Jackson and Rogers node inclusion model, the fourth a Watts and Strogatz
	'small world' model. 
	
	For the SAOM-related models additional model specifications are necessary.
}
\usage{
	create_multinomial_choice_network_change_model(
				focalActorIndex, networkIndex, effectContainer)
	create_multinomial_choice_behavior_change_model(
				focalActorIndex, attributeIndex, effectContainer)
	create_jackson_rogers_change_model(
				networkIndex, 
				pLinkToParentNode = 1.0, pLinkToNeigborNode = 1.0, 
				nParentNodes = 1, nNeighborNodes = 1)
	create_watts_strogatz_change_model(networkIndex)
	create_attribute_multinomial_choice_network_change_model(
				networkIndex, poissonAttributeIndex, 
				updater = create_tie_swap_updater(networkIndex))
}
\arguments{
	\item{focalActorIndex}{Index of focal actor of the multinomial (network or behavior) change model}
	\item{networkIndex}{Index of the network that is subject to change (dependent network)}
	\item{effectContainer}{Container object storing effects as defined in the SIENA manual}
	\item{attributeIndex}{Index of the attribute container that is subject to change (dependent attribute)}
	\item{pLinkToParentNode}{Probability to link to a 'parent node'. See Jackson \& Rogers (2007), p.894}
	\item{pLinkToNeigborNode}{Probability to link to a selected neighbor of a parent. See Jackson \& Rogers (2007), p.894}
	\item{nParentNodes}{Number of 'parent node'. See Jackson \& Rogers (2007), p.894}
	\item{nNeighborNodes}{Number of neighbors considered of parent node. See Jackson \& Rogers (2007), p.894}
	\item{updater}{Defines a change that is considered by the focal actor after choosing a particular tie. By default, this is a tie swap updater.}
	\item{poissonAttributeIndex}{Index of a attribute container containing a number of concurring Poisson parameters}
}
\seealso{
	\code{\link{add_change_model}}
	\code{\link{create_tie_swap_updater}}
	\code{\link{create_effect_container}}
	\code{\link{add_to_effect_container}}
	\code{\link{create_effect}}
	\code{\link{add_effect}}
}
\references{
Ripley, R. M.; Snijders, T. A. B. & Preciado Lopez, P.: Manual for SIENA 4.0. (2012), 
Oxford: University of Oxford, Department of Statistics; Nuffield College. 
URL: http://www.stats.ox.ac.uk/~snijders/siena/RSiena_Manual.pdf

Jackson, M. O. & Rogers, B. W. Meeting strangers and friends of friends: How random are social networks? American Economic Review, 2007, 97, 890-915

Watts, D. J. & Strogatz, S. H. Collective dynamics of 'small-world' networks Nature, 1998, 393, 440-442
}
\examples{
## The following example is taken from section 4.1 and 4.2 in package vignette
## C. Stadtfeld: "Netsim: A Social Networks Simulation tool in R" (2013)
## Examples of the other models can be taken from that source
 
# construct process state
nActors <- 21
mat <- matrix(0, nActors, nActors)
att <- c(rep(0, nActors/3),rep(1, nActors/3),rep(2, nActors/3))
network <- create_network(mat)
attributeContainer <- create_scale_attribute_container(att,
min=0, max=3, by = 1)
processState <- create_process_state()
processState <- add_network(processState, network, name = "network")
processState <- add_attribute_container(processState, attributeContainer,
name = "attribute")
networkIndex <- get_network_index(processState)
attributeIndex <- get_attribute_container_index(processState)

# construct effect container
# homophily model with basic network effects
effectContainerHomophily <- create_effect_container()
effectContainerHomophily <- add_to_effect_container(
	effectContainerHomophily,
	create_effect("density", networkIndex),
	-2.5)
effectContainerHomophily <- add_to_effect_container(
	effectContainerHomophily,
	create_effect("recip", networkIndex),
	2.5)
effectContainerHomophily <- add_to_effect_container(
	effectContainerHomophily,
	create_effect("transTrip", networkIndex),
	0.5)
effectContainerHomophily <- add_to_effect_container(
	effectContainerHomophily,
	create_effect("cycle3", networkIndex),
	-0.5)
effectContainerHomophily <- add_to_effect_container(
	effectContainerHomophily,
	create_effect("simX",
		attributeIndex,
		networkIndex,
		0.5),
	1.0)
	
# effect container of the behavior change model
effectContainerInfluence <- create_effect_container()
effectContainerInfluence <- add_to_effect_container(
	effectContainerInfluence,
	create_effect("linear",
		attributeIndex),
	0.0)
effectContainerInfluence <- add_to_effect_container(
	effectContainerInfluence,
	create_effect("quad",
		attributeIndex),
	0.0)
effectContainerInfluence <- add_to_effect_container(
	effectContainerInfluence,
	create_effect("totSim",
		attributeIndex,
		networkIndex, 10/18),
	2.0)

# Definition of SAOM network model manager
modelManager <- create_model_manager()
# assign homophily model to all actors
for (i in c(0 : (nActors - 1) ) ){
	# Poisson models
	poissonParameterInfluence <- 5
	poissonModelInfluence <- create_poisson_model(
	poissonParameterInfluence)
	
	poissonParameter <- 40
	poissonModel <- create_poisson_model(poissonParameter)
	
	# saom change models
	saomHomophilyModel <- create_multinomial_choice_network_change_model(
		i,
		networkIndex,
		effectContainerHomophily)
	behaviorSaom <- create_multinomial_choice_behavior_change_model(
		i,
		attributeIndex,
		effectContainerInfluence
	)
	# updaters
	setAttributeUpdater <- create_actor_attribute_set_updater(
	attributeIndex, i)
		
	tieSwapUpdater <- create_tie_swap_updater(networkIndex)
	
	# define model chains
	modelManager <<- add_time_model(modelManager,
		poissonModel)
	modelManager <<- add_change_model(modelManager,
		poissonModel,
		saomHomophilyModel)
	modelManager <<- add_updater(modelManager,
		saomHomophilyModel,
		tieSwapUpdater)
	modelManager <<- add_time_model(modelManager,
        poissonModelInfluence)
	modelManager <<- add_change_model(modelManager,
        poissonModelInfluence,
        behaviorSaom)
	modelManager <<- add_updater(modelManager,
        behaviorSaom,
        setAttributeUpdater)
		
} # for loop

simulator <- create_simulator(processState, modelManager, 10)
# commented out due to Windows compilation problems. Further tests needed!
\dontrun{simulate(simulator)}

}