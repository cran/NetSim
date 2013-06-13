/*
 * rwrapper_attribute_container.cpp
 *
 *  Created on: Jan 26, 2013
 *      Author: cws
 */

#include "rwrapper_attribute_container.h"

SEXP create_attribute_container(SEXP list_) {
	BEGIN_RCPP

	// transform numeric vector into double vector
	Rcpp::NumericVector list(list_);
	int vectorLength = list.size();
	std::vector<double> valuesVector(vectorLength,0);
	for (int i = 0; i< vectorLength; i++){
		valuesVector[i] = list(i);
	}

	AttributeContainer * attributeContainer = new AttributeContainer(valuesVector);

	Rcpp::XPtr<AttributeContainer> pointer(attributeContainer, false);
	pointer.attr("class") = "NetSimAttributeContainer";
	return pointer;

	END_RCPP
}

SEXP create_scale_attribute_container(SEXP list_, SEXP min_, SEXP max_, SEXP by_) {
	BEGIN_RCPP

	// transform numeric vector into double vector
	Rcpp::NumericVector list(list_);
	int vectorLength = list.size();
	std::vector<double> valuesVector(vectorLength,0);
	for (int i = 0; i< vectorLength; i++){
		valuesVector[i] = list(i);
	}

	double min = Rcpp::as<double>(min_);
	double max = Rcpp::as<double>(max_);
	double by = Rcpp::as<double>(by_);

	AttributeContainer * attributeContainer = new ScaleAttributeContainer(
			valuesVector, min, max, by);

	Rcpp::XPtr<AttributeContainer> pointer(attributeContainer, false);
	pointer.attr("class") = "NetSimAttributeContainer";
	return pointer;

	END_RCPP
}


SEXP attribute_container_as_list(SEXP attributeContainer_) {
	BEGIN_RCPP

	AttributeContainer * attributeContainer = Rcpp::XPtr<AttributeContainer>(attributeContainer_);
	int nActors = attributeContainer->getSize();

	Rcpp::NumericVector list(nActors);

	for (int i = 0; i < nActors; i++){
		list(i) = attributeContainer->getValue(i);
	}

	return list;

	END_RCPP
}

SEXP set_value(SEXP attributeContainer_, SEXP i_, SEXP value_) {
	BEGIN_RCPP

	AttributeContainer * attributeContainer = Rcpp::XPtr<AttributeContainer>(attributeContainer_);
	int i = Rcpp::as<int>(i_);
	double value = Rcpp::as<double>(value_);

	attributeContainer->setValue(i, value);

	Rcpp::XPtr<AttributeContainer> pointer(attributeContainer, false);
	pointer.attr("class") = "NetSimAttributeContainer";
	return pointer;

	END_RCPP
}
