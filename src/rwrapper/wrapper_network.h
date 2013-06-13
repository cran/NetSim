/*
 * wrapper_network.h
 *
 *  Created on: Jan 20, 2013
 *      Author: cws
 */

#ifndef WRAPPER_NETWORK_H_
#define WRAPPER_NETWORK_H_

#include <Rcpp.h>
#include <stdexcept>
#include "../network/MemoryOneModeNetwork.h"
#include "../network/NetworkUtils.h"

RcppExport SEXP create_network(SEXP matrix_, SEXP directed_, SEXP reflexive_);

RcppExport SEXP network_as_matrix(SEXP networkPointer_);

RcppExport SEXP set_tie(SEXP network, SEXP i, SEXP j, SEXP value);

RcppExport SEXP add_random_ties_to_network(SEXP network, SEXP probability);

RcppExport SEXP add_ring_lattice_to_network(SEXP network, SEXP nReciprocalTies);

#endif /* WRAPPER_NETWORK_H_ */
