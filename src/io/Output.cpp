/*
 * Output.cpp
 *
 *  Created on: Aug 30, 2013
 *      Author: cws
 */

#include "Output.h"

void Output::out(std::string str) {
	Rcpp::Rcout << str;
}

void Output::flush() {
	R_FlushConsole();
}

void Output::endl() {
	Rcpp::Rcout << std::endl;
}

Output::Output() {
	//RInstanceFactory::getInstance();	// careful: this is not part of the R package!
}
