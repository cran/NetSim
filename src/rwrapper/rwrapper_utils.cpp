/*
 * rwrapper_utils.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: cws
 */

#include "rwrapper_utils.h"

SEXP reset_random_seed(SEXP seed_) {
	BEGIN_RCPP

	int seed = Rcpp::as<int>(seed_);
	if ( seed < 0 ) return Rcpp::wrap(-1);

	Random::getInstance().resetSeed(seed);

	return Rcpp::wrap(1);

	END_RCPP
}

SEXP get_random_seed() {
	BEGIN_RCPP

	int seed = Random::getInstance().getSeed();

	return Rcpp::wrap(seed);

	END_RCPP
}
