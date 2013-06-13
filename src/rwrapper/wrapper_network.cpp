/*
 * wrapper_network.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: cws
 */

#include "wrapper_network.h"

SEXP create_network(SEXP matrix_, SEXP directed_, SEXP reflexive_) {
	BEGIN_RCPP

	Rcpp::NumericMatrix matrix(matrix_);
	int nCol = matrix.ncol();
	int nRow = matrix.nrow();

	bool directed = Rcpp::as<bool>(directed_);
	bool reflexive = Rcpp::as<bool>(reflexive_);

	if (nCol != nRow) {
		throw std::invalid_argument("Matrix needs to be quadratic.");
		return R_NilValue;
	}

	MemoryOneModeNetwork * network = new MemoryOneModeNetwork(nCol, directed, reflexive);

	// check if there are problems when ties are set (setTie == false)
	int problematicTies = 0;
	for (int i = 0; i < nRow; i++){
		for (int j = 0; j < nCol; j++){
			if(!network->setTie(i, j,  matrix(i, j) ))
				++problematicTies;
		}
	}
	if (problematicTies > 0)
		Rcpp::Rcout << "Warning: " << problematicTies <<
			" ties could not be set." <<std::endl;

	Rcpp::XPtr<MemoryOneModeNetwork> pointer(network, false);
	pointer.attr("class") = "NetSimNetwork";
	return pointer;


	END_RCPP
}

SEXP network_as_matrix(SEXP networkPointer_) {
	BEGIN_RCPP

	Rcpp::XPtr<MemoryOneModeNetwork> network(networkPointer_);

	int size = network->getSize();

	Rcpp::NumericMatrix matrix(size, size);

	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			matrix(i,j) = network->getTieValue(i, j);
		}
	}

	return matrix;

	END_RCPP
}

SEXP set_tie(SEXP network_, SEXP i_, SEXP j_, SEXP value_) {
	BEGIN_RCPP

		Rcpp::XPtr<MemoryOneModeNetwork> network(network_);

		int i = Rcpp::as<int>(i_);
		int j = Rcpp::as<int>(j_);
		double value = Rcpp::as<double>(value_);

		bool tieSet = network->setTie(i, j, value);

		return Rcpp::wrap(tieSet);

		END_RCPP
}

SEXP add_random_ties_to_network(SEXP network_, SEXP probability_) {
	BEGIN_RCPP

		Rcpp::XPtr<MemoryOneModeNetwork> network(network_);

		double probabilty = Rcpp::as<double>(probability_);

		NetworkUtils::addRandomTiesToNetwork(network, probabilty);

		Rcpp::XPtr<MemoryOneModeNetwork> pointer(network, false);
		pointer.attr("class") = "NetSimNetwork";
		return pointer;

		END_RCPP
}

SEXP add_ring_lattice_to_network(SEXP network_, SEXP nReciprocalTies_) {
	BEGIN_RCPP

	Rcpp::XPtr<MemoryOneModeNetwork> network(network_);
	int nReciprocalTies = Rcpp::as<int>(nReciprocalTies_);

	NetworkUtils::addRingLatticeTiesToNetwork(network, nReciprocalTies);

	Rcpp::XPtr<MemoryOneModeNetwork> pointer(network, false);
	pointer.attr("class") = "NetSimNetwork";
	return pointer;

	END_RCPP
}
