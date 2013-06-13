/*
 * JacksonRogers2007ChangeModel.h
 *
 *  Created on: Feb 13, 2013
 *      Author: cws
 */

#ifndef JACKSONROGERS2007CHANGEMODEL_H_
#define JACKSONROGERS2007CHANGEMODEL_H_

#include "ModelResult.h"
#include "ChangeModel.h"
#include "../network/NetworkUtils.h"
#include "../io/Output.h"

/**
 * Implementation of the change model from
 * Jackson, Matthew O. AND Rogers, Brian W. (J&R)
 * "Meeting strangers and friends of friends:
 *  How random are social networks?"
 * American Economic Review 97, 2007
 */
class JacksonRogers2007ChangeModel : public ChangeModel{

public:
	JacksonRogers2007ChangeModel(
			int networkIndex,
			double pLinkToParentNode,
			double pLinkToNeigborNode,
			int nParentNodes,
			int nNeighborNodes);

	ModelResult * getChange(ProcessState * processState);

	void setDebug(bool debug = true);

private:
	int _networkIndex;
	// p_r on page 894
	double _pLinkToParentNode;
	// p_n on page 894
	double _pLinkToNeigborNode;
	// m_r on page 894
	int _nParentNodes;
	// m_n on page 894
	int _nNeighborNodes;

	double _debug;

};




#endif /* JACKSONROGERS2007CHANGEMODEL_H_ */
