/*
 * WattsStrogatzRandomRewiringChangeModel.h
 *
 *  Created on: Feb 26, 2013
 *      Author: cws
 */

#ifndef WATTSSTROGATZRANDOMREWIRINGCHANGEMODEL_H_
#define WATTSSTROGATZRANDOMREWIRINGCHANGEMODEL_H_

#include "ChangeModel.h"
#include "ModelResult.h"
#include "../network/MemoryOneModeNetwork.h"
#include "../utils/Random.h"
#include "../network/NetworkUtils.h"
#include <stdexcept>
#include "../io/Output.h"

class WattsStrogatzRandomRewiringChangeModel : public ChangeModel{

public:
	WattsStrogatzRandomRewiringChangeModel(size_t networkIndex);

	ModelResult * getChange(ProcessState * processState);

	void setDebug(bool debug = true);

private:
	size_t _networkIndex;
	bool _debug;
};



#endif /* WATTSSTROGATZRANDOMREWIRINGCHANGEMODEL_H_ */
