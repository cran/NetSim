/*
 * wrapper_process_state.h
 *
 *  Created on: Jan 11, 2013
 *      Author: cws
 */

#ifndef CREATE_PROCESS_STATE_H_
#define CREATE_PROCESS_STATE_H_

#include <Rcpp.h>
#include <string>
#include "../processstate/ProcessState.h"
#include "../processstate/ProcessStateManager.h"
#include "../network/MemoryOneModeNetwork.h"

RcppExport SEXP create_process_state(SEXP name);

RcppExport SEXP get_process_state_name(SEXP pointer);

RcppExport SEXP add_network(SEXP processStateManager, SEXP network, SEXP name);

RcppExport SEXP get_network_index(SEXP processStateManager, SEXP name);

RcppExport SEXP get_network(SEXP processStateManager, SEXP name);

RcppExport SEXP add_attribute_container(SEXP processStateManager, SEXP attributeContainer, SEXP name);

RcppExport SEXP get_attribute_container_index(SEXP processStateManager, SEXP name);

RcppExport SEXP get_attribute_container(SEXP processStateManager, SEXP name);

RcppExport SEXP add_global_attribute(SEXP processStateManager, SEXP value, SEXP name);

RcppExport SEXP get_global_attribute_index(SEXP processStateManager, SEXP name);

RcppExport SEXP get_global_attribute(SEXP processStateManager, SEXP name);

#endif /* CREATE_PROCESS_STATE_H_ */
