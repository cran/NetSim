/*
 * Output.h
 *
 *  Created on: Aug 30, 2013
 *      Author: cws
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <Rcpp.h>
#include <iostream>
// #include "../utils/RInstanceFactory.h"

/**
 * Output class that takes care of providing the right output formats
 * depending on the environment the package if run from (R, CUTE).
 */
class Output{

public:

	static void out(std::string str);

	static void flush();

	static void endl();


	template<typename T> Output &operator << (const T &t){
		Rcpp::Rcout << t;
		return * this;
	}

	//	static void err(std::string str);

	Output();

	~Output(){ }


private:


};


#endif /* OUTPUT_H_ */
