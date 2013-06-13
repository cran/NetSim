/*
 * wrapper_model.h
 *
 *  Created on: Jan 22, 2013
 *      Author: cws
 */

#ifndef WRAPPER_MODEL_H_
#define WRAPPER_MODEL_H_


#include <Rcpp.h>
#include "../model/ModelManager.h"
#include "../model/TimeModel.h"
#include "../model/ChangeModel.h"
#include "../model/MultinomialChoiceNetworkChangeModel.h"
#include "../model/MultinomialChoiceBehaviorChangeModel.h"
#include "../model/Updater.h"
#include "../model/saom/SaomEffect.h"
#include "../model/saom/EffectContainerManager.h"
#include "../model/saom/EffectFactory.h"
#include "../model/JacksonRogers2007ChangeModel.h"
#include "../model/WattsStrogatzRandomRewiringChangeModel.h"

RcppExport SEXP create_model_manager();

RcppExport SEXP add_time_model(SEXP modelManager, SEXP timeModel);

RcppExport SEXP add_change_model(SEXP modelManager, SEXP timeModel, SEXP changeModel);

RcppExport SEXP add_updater(SEXP modelManager, SEXP changeModel, SEXP updater);

RcppExport SEXP add_time_updater(SEXP modelManager, SEXP timeUpdater);

RcppExport SEXP create_poisson_model(SEXP param);

RcppExport SEXP create_attribute_poisson_model(SEXP attributeIndex);

// for pre-defined SAOMs
RcppExport SEXP add_effect_with_parameter(SEXP saom, SEXP effect, SEXP parameter);

// for saoms that use actor attributes
RcppExport SEXP add_effect_with_index(SEXP saom, SEXP effect, SEXP index);

RcppExport SEXP create_effect_container();

RcppExport SEXP create_one_mode_effect(SEXP name, SEXP networkIndex);

RcppExport SEXP create_attribute_one_mode_effect(SEXP name, SEXP attributeIndex, SEXP networkIndex);

RcppExport SEXP create_similarity_attribute_one_mode_effect(SEXP name,
		SEXP attributeIndex,
		SEXP networkIndex,
		SEXP meanSimilarityScores);

RcppExport SEXP create_attribute_effect(SEXP name, SEXP attributeIndex);

RcppExport SEXP create_multiplex_network_effect(SEXP name,
		SEXP dependentNetworkIndex,
		SEXP secondNetworkIndex);

RcppExport SEXP get_effect_type(SEXP name);

RcppExport SEXP add_to_effect_container(SEXP effectContainer, SEXP effect, SEXP paramValue);

/**
 * Multinomial choice network change model.
 * So far, this only implements one updater instead of an updater list.
 */
RcppExport SEXP create_multinomial_choice_network_change_model(
		SEXP focalActorIndex, SEXP networkIndex, SEXP effectContainer, SEXP updater);


// size_t dependentNetworkIndex,
// size_t poissonAttributeIndex, // to determine focal actor
// std::vector<Updater*> updaters);
RcppExport SEXP create_attribute_multinomial_choice_network_change_model(
		SEXP networkIndex, SEXP poissonAttributeIndex, SEXP updater);


RcppExport SEXP create_multinomial_choice_behavior_change_model(
		SEXP focalActorIndex, SEXP attributeIndex, SEXP effectContainer);

RcppExport SEXP create_tie_swap_updater(SEXP networkIndex);

RcppExport SEXP create_actor_attribute_set_updater(SEXP attributeIndex, SEXP actorIndex);

RcppExport SEXP create_jackson_rogers_change_model(
		SEXP networkId, SEXP pLinkToParentNode, SEXP pLinkToNeighborNode,
		SEXP nParentNodes, SEXP nNeighborNodes);

RcppExport SEXP create_watts_strogatz_change_model(SEXP networkId);

RcppExport SEXP create_rewire_tie_updater(SEXP networkId);

RcppExport SEXP create_round_based_time_model(
		SEXP timerIndex, SEXP intervalLength, SEXP startTime);

RcppExport SEXP create_add_actor_updater();

RcppExport SEXP create_add_ties_from_newborn_actor_updater(
		SEXP networkIndex);

RcppExport SEXP create_set_attribute_of_newborn_actor_updater(
		SEXP attributeIndex, SEXP value);

RcppExport SEXP create_timer_updater(SEXP timerIndex);


#endif /* WRAPPER_MODEL_H_ */
