/*
 * rwrapper_attribute_container.h
 *
 *  Created on: Jan 26, 2013
 *      Author: cws
 */

#ifndef RWRAPPER_ATTRIBUTE_CONTAINER_H_
#define RWRAPPER_ATTRIBUTE_CONTAINER_H_

#include <Rcpp.h>
#include <string>
#include <stdexcept>
#include <vector>
#include "../attribute/AttributeContainer.h"

RcppExport SEXP create_attribute_container(SEXP list);

RcppExport SEXP create_scale_attribute_container(SEXP list, SEXP min,
		SEXP max, SEXP by);

RcppExport SEXP attribute_container_as_list(SEXP attributeContainer);

RcppExport SEXP set_value(SEXP attributeContainer, SEXP i, SEXP value);


#endif /* RWRAPPER_ATTRIBUTE_CONTAINER_H_ */
