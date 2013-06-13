/*
 * rwrapper_utils.h
 *
 *  Created on: Aug 26, 2013
 *      Author: cws
 */

#ifndef RWRAPPER_UTILS_H_
#define RWRAPPER_UTILS_H_

#include <Rcpp.h>
#include "../utils/Random.h"

RcppExport SEXP reset_random_seed(SEXP seed);

RcppExport SEXP get_random_seed();

#endif /* RWRAPPER_UTILS_H_ */
