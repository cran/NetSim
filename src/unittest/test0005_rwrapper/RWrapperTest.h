/*
 * RWrapperTest.h
 *
 *  Created on: Jan 21, 2013
 *      Author: cws
 */

#ifndef RWRAPPERTEST_H_
#define RWRAPPERTEST_H_

#include "cute.h"
#include <RInside.h>
#include <Rcpp.h>
#include "../../utils/RInstanceFactory.h"
#include "../io/Output.h"

// to be only called once
static RInside * createRInstance(){
	RInside R;
	return &R;
}


void matrixToNetworkRWrapperTest(){

	RInside R = RInstanceFactory::getInstance().getRInside();

	R.parseEvalQ("library(NetSim)");
	R.parseEvalQ("mat <- matrix(1,2,2)");
	R.parseEvalQ("network <- create_network(mat, reflexive = FALSE)");
	R.parseEvalQ("newMatrix <- network_as_matrix(network)");
	SEXP myMatrix = R["newMatrix"];


	Rcpp::NumericMatrix matrix(myMatrix);

	ASSERT(matrix(0,0) == 0);
	ASSERT(matrix(1,0) == 1);
	ASSERT(matrix(0,1) == 1);
	ASSERT(matrix(1,1) == 0);

	Rcpp::Language printMatrix("print",myMatrix);
	printMatrix.eval();


}

void rOutTest(){

	Output() << "Output in R";
}

cute::suite getRWrapperTestSuite(){

//	createRInstance();

	cute::suite s;

	s.push_back(CUTE(matrixToNetworkRWrapperTest));
	s.push_back(CUTE(rOutTest));

	return s;
}


#endif /* RWRAPPERTEST_H_ */
