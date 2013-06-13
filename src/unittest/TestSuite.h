/*
 *
 *  Created on: Nov 21, 2012
 *      Author: cws
 */

#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include "../attribute/AttributeContainer.h"
#include "test0001_network/MemoryOneModeNetworkTest.h"
#include "test0002_attribute/AttributeContainerTest.h"
#include "test0003_saom/SaomTest.h"
#include "test0004_effects/EffectTest.h"
#include "test0005_rwrapper/RWrapperTest.h"
#include "test0006_mementotest/MementoTest.h"
#include "test0007_jacksonrogers/JacksonRogersTest.h"
#include "test0008_timemodel/TimeModelTest.h"
#include "test0009_wattsstrogatz/WattsStrogatzTest.h"
#include "test0010_utils/UtilsTest.h"


void runUnitTestSuite(){
	cute::suite s0001 = getTestSuiteMemoryOneModeNetwork();
	cute::suite s0002 = getTestAttributeContainerSuite();
	cute::suite s0003 = getTestSaomSuite();
	cute::suite s0004 = getTestSaomEffectsSuite();
	cute::suite s0005 = getRWrapperTestSuite();
	cute::suite s0006 = getMementoTests();
	cute::suite s0007 = getJacksonRogersTests();
	cute::suite s0008 = getTimeModelTests();
	cute::suite s0009 = getWattsStrogatzTests();
	cute::suite s0010 = getUtilsTests();
	cute::ide_listener idl;
	cute::makeRunner(idl)(s0001, "Network tests");
	cute::makeRunner(idl)(s0002, "Attribute Container tests");
	cute::makeRunner(idl)(s0003, "SAOM tests");
	cute::makeRunner(idl)(s0004, "SAOM Effect tests");
	cute::makeRunner(idl)(s0005, "R wrapper tests");
	cute::makeRunner(idl)(s0006, "Memento tests");
	cute::makeRunner(idl)(s0007, "Jackson Rogers model tests");
	cute::makeRunner(idl)(s0008, "Time Model tests");
	cute::makeRunner(idl)(s0009, "Watts Strogatz tests");
	cute::makeRunner(idl)(s0010, "Utils tests");

}






#endif /* TESTSUITE_H_ */
