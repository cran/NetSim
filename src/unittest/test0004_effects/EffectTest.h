/*
 * EffectTest.h
 *
 *  Created on: Jan 9, 2013
 *      Author: cws
 */

#ifndef EFFECTTEST_H_
#define EFFECTTEST_H_

#include "../../model/saom/SaomEffect.h"
#include "../../processstate/ProcessState.h"
#include "../../network/MemoryOneModeNetwork.h"


MemoryOneModeNetwork getSimpleNetwork(){
	MemoryOneModeNetwork network(5);
	network.addTie(0,1);
	network.addTie(0,2);
	network.addTie(0,3);
	return network;
}

MemoryOneModeNetwork getSimpleNetwork2(){
	MemoryOneModeNetwork network(5);
	network.addTie(0,1);
	network.addTie(1,2);
	network.addTie(2,3);
	network.addTie(0,2);
	network.addTie(3,4);
	network.addTie(2,4);
	return network;
}

// complete network with three nodes
MemoryOneModeNetwork getSimpleNetwork3(){
	MemoryOneModeNetwork network(3);
	network.addTie(0,1);
	network.addTie(1,0);
	network.addTie(0,2);
	network.addTie(2,0);
	network.addTie(1,2);
	network.addTie(2,1);
	return network;
}


void densityEffectTest(){
	ProcessState processState;
	MemoryOneModeNetwork network = getSimpleNetwork();
	size_t indexNetwork = processState.addNetwork(&network);

	DensityEffect densityEffect(indexNetwork);
	ASSERT_EQUAL(3, densityEffect.getEffect(&processState, 0));
	ASSERT_EQUAL(0, densityEffect.getEffect(&processState, 1));
}


void twoPathEffectTest(){
	ProcessState processState;
	MemoryOneModeNetwork network = getSimpleNetwork2();
	size_t indexNetwork = processState.addNetwork(&network);

	TwoPathEffect twoPathEffect(indexNetwork);
	ASSERT_EQUAL(2, twoPathEffect.getEffect(&processState, 0));
	ASSERT_EQUAL(2, twoPathEffect.getEffect(&processState, 1));
	ASSERT_EQUAL(0, twoPathEffect.getEffect(&processState, 2));
	// repeat to make sure that the internal sets are not changed
	ASSERT_EQUAL(2, twoPathEffect.getEffect(&processState, 0));
	ASSERT_EQUAL(2, twoPathEffect.getEffect(&processState, 1));
	ASSERT_EQUAL(0, twoPathEffect.getEffect(&processState, 2));

}

void transTripEffectTest(){
	ProcessState processState;
	MemoryOneModeNetwork network = getSimpleNetwork2();
	size_t indexNetwork = processState.addNetwork(&network);

	TransitivityEffect transTripEffect(indexNetwork);
	ASSERT_EQUAL(1, transTripEffect.getEffect(&processState, 0));
	ASSERT_EQUAL(0, transTripEffect.getEffect(&processState, 1));
	ASSERT_EQUAL(1, transTripEffect.getEffect(&processState, 2));

	MemoryOneModeNetwork network2 = getSimpleNetwork3();
	size_t indexNetwork2 = processState.addNetwork(&network2);

	TransitivityEffect transTripEffect2(indexNetwork2);
	ASSERT_EQUAL(2, transTripEffect2.getEffect(&processState, 0));

}

void threeCycleEffectTest(){
	ProcessState processState;
	MemoryOneModeNetwork network = getSimpleNetwork2();
	size_t indexNetwork = processState.addNetwork(&network);

	ThreeCycleEffect threeCycleEffect(indexNetwork);
	ASSERT_EQUAL(0, threeCycleEffect.getEffect(&processState, 0));
	ASSERT_EQUAL(0, threeCycleEffect.getEffect(&processState, 1));
	ASSERT_EQUAL(0, threeCycleEffect.getEffect(&processState, 2));
	network.addTie(2,0);
	ASSERT_EQUAL(1, threeCycleEffect.getEffect(&processState, 0));
	network.addTie(4,0);
	ASSERT_EQUAL(2, threeCycleEffect.getEffect(&processState, 0));
	network.addTie(3,0);
	ASSERT_EQUAL(3, threeCycleEffect.getEffect(&processState, 0));
}

void reciprocityEffectTest(){
	ProcessState processState;
	MemoryOneModeNetwork network = getSimpleNetwork3();
	size_t indexNetwork = processState.addNetwork(&network);

	ReciprocityEffect reciprocityEffect(indexNetwork);
	ASSERT_EQUAL(2, reciprocityEffect.getEffect(&processState, 0));
	network.removeTie(1,0);
	ASSERT_EQUAL(1, reciprocityEffect.getEffect(&processState, 0));
	network.removeTie(0,2);
	ASSERT_EQUAL(0, reciprocityEffect.getEffect(&processState, 0));
	network.addTie(1,0);
	ASSERT_EQUAL(1, reciprocityEffect.getEffect(&processState, 0));
	network.addTie(1,0);
	ASSERT_EQUAL(1, reciprocityEffect.getEffect(&processState, 0));

}

void inPopularityEffectTest(){
	ProcessState processState;
	MemoryOneModeNetwork network = getSimpleNetwork3();
	size_t indexNetwork = processState.addNetwork(&network);

	InPopularityEffect inPopEffect(indexNetwork);
	ASSERT_EQUAL(4, inPopEffect.getEffect(&processState, 0));
	network.removeTie(1,0);
	ASSERT_EQUAL(4, inPopEffect.getEffect(&processState, 0));
	network.removeTie(0,1);
	ASSERT_EQUAL(2, inPopEffect.getEffect(&processState, 0));
	network.removeTie(1,2);
	ASSERT_EQUAL(1, inPopEffect.getEffect(&processState, 0));
}

void outPopularityEffectTest(){
	ProcessState processState;
	MemoryOneModeNetwork network = getSimpleNetwork3();
	size_t indexNetwork = processState.addNetwork(&network);

	OutPopularityEffect outPopEffect(indexNetwork);
	ASSERT_EQUAL(4, outPopEffect.getEffect(&processState, 0));
	network.removeTie(1,0);
	ASSERT_EQUAL(3, outPopEffect.getEffect(&processState, 0));
	network.removeTie(0,1);
	ASSERT_EQUAL(2, outPopEffect.getEffect(&processState, 0));
	network.removeTie(1,2);
	ASSERT_EQUAL(2, outPopEffect.getEffect(&processState, 0));
}


cute::suite getTestSaomEffectsSuite(){
	cute::suite s;

	s.push_back(CUTE(densityEffectTest));
	s.push_back(CUTE(reciprocityEffectTest));
	s.push_back(CUTE(transTripEffectTest));
	s.push_back(CUTE(threeCycleEffectTest));
	s.push_back(CUTE(twoPathEffectTest));
	s.push_back(CUTE(inPopularityEffectTest));
	s.push_back(CUTE(outPopularityEffectTest));

	return s;
}


#endif /* EFFECTTEST_H_ */
