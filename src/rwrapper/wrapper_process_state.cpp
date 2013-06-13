
/*
 * wrapper_process_state.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: cws
 */

#include "wrapper_process_state.h"

SEXP create_process_state(SEXP n) {
	BEGIN_RCPP

	Rcpp::CharacterVector name(n);
	std::string nameString =  Rcpp::as<std::string>(name);

	ProcessStateManager * processStateManager = new ProcessStateManager(nameString);

	Rcpp::XPtr<ProcessStateManager> pointer(processStateManager, false);

	pointer.attr("class") = "processState";

	return pointer;

	END_RCPP
}


SEXP get_process_state_name(SEXP pointer) {
	BEGIN_RCPP

	Rcpp::XPtr<ProcessStateManager> processStateManager(pointer);
	std::string name = processStateManager->getName();

	return Rcpp::wrap(name);

	END_RCPP
}

SEXP add_network(SEXP processStateManager_, SEXP network_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);

	MemoryOneModeNetwork * network =
			Rcpp::XPtr<MemoryOneModeNetwork>(network_);
	std::string name = Rcpp::as<std::string>(name_);

	processStateManager->addNetwork(network, name);

	Rcpp::XPtr<ProcessStateManager> pointer(processStateManager, false);
	pointer.attr("class") = "processState";
	return pointer;

	END_RCPP
}

SEXP get_network_index(SEXP processStateManager_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	std::string name = Rcpp::as<std::string>(name_);

	size_t index = processStateManager->getNetworkIndex(name);

	// wrapping size_t values is not supported by Rcpp
	int intIndex = (int) index;
	return Rcpp::wrap(intIndex);

	END_RCPP
}

SEXP get_network(SEXP processStateManager_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	std::string name = Rcpp::as<std::string>(name_);

	Network * network = processStateManager->getNetwork(name);

	// TODO: pointer is not properly typed
	return Rcpp::XPtr<Network>(network, false);

	END_RCPP
}

SEXP add_attribute_container(SEXP processStateManager_, SEXP attributeContainer_,
		SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);

	AttributeContainer * attributeContainer =
			Rcpp::XPtr<AttributeContainer>(attributeContainer_);
	std::string name = Rcpp::as<std::string>(name_);

	processStateManager->addAttributeContainter(attributeContainer, name);

	Rcpp::XPtr<ProcessStateManager> pointer(processStateManager, false);
	pointer.attr("class") = "processState";
	return pointer;


	END_RCPP
}

SEXP get_attribute_container_index(SEXP processStateManager_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	std::string name = Rcpp::as<std::string>(name_);

	size_t index = processStateManager->getAttributeContainerIndex(name);

	// wrapping size_t values is not supported by Rcpp
	int intIndex = (int) index;
	return Rcpp::wrap(intIndex);


	END_RCPP
}

SEXP get_attribute_container(SEXP processStateManager_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	std::string name = Rcpp::as<std::string>(name_);

	AttributeContainer * attributeContainer = processStateManager->getAttributeContainer(name);

	// TODO: pointer is not properly typed
	return Rcpp::XPtr<AttributeContainer>(attributeContainer, false);

	END_RCPP
}

SEXP add_global_attribute(SEXP processStateManager_, SEXP value_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	double value = Rcpp::as<double>(value_);
	std::string name = Rcpp::as<std::string>(name_);

	processStateManager->addGlobalAttribute(value, name);

	return Rcpp::XPtr<ProcessStateManager>(processStateManager, false);


	END_RCPP
}

SEXP get_global_attribute_index(SEXP processStateManager_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	std::string name = Rcpp::as<std::string>(name_);

	// wrapping size_t values is not supported by Rcpp
	size_t index = processStateManager->getGlobalAttributeIndex(name);
	int intIndex = (int) index;
	return Rcpp::wrap(intIndex);


	END_RCPP
}

SEXP get_global_attribute(SEXP processStateManager_, SEXP name_) {
	BEGIN_RCPP

	ProcessStateManager * processStateManager =
			Rcpp::XPtr<ProcessStateManager>(processStateManager_);
	std::string name = Rcpp::as<std::string>(name_);

	double v = processStateManager->getGlobalAttribute(name);
	return Rcpp::wrap(v);

	END_RCPP
}
