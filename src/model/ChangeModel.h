/*
 * ChangeModel.h
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#ifndef CHANGEMODEL_H_
#define CHANGEMODEL_H_

#include "ModelResult.h"
#include "../network/NetworkUtils.h"
#include "../processstate/ProcessState.h"
#include <utility>

class ChangeModel{

public:
	virtual ~ChangeModel(){ }
	virtual ModelResult * getChange(ProcessState * processState) = 0;

	void setDebug(bool debug);

protected:
	bool _debug;
};

class NullChangeModel : public ChangeModel{
	ModelResult * getChange(ProcessState * processState);
};

class ChooseRandomTieChangeModel : public ChangeModel{
public:
	ChooseRandomTieChangeModel(size_t networkIndex);
	ModelResult * getChange(ProcessState * processState);
private:
	size_t _networkIndex;
};


#endif /* CHANGEMODEL_H_ */
