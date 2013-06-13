/*
 * TimeModelTest.h
 *
 *  Created on: Feb 26, 2013
 *      Author: cws
 */

#ifndef TIMEMODELTEST_H_
#define TIMEMODELTEST_H_

#include "cute.h"
#include "TimeModel.h"


void roundBasedTimeModelTest(){
	double startTime = 1.5;
	double interval = 1.0;

	ProcessState * processState = new ProcessState();
	size_t timerIndex = processState->addGlobalAttribute(0.0);

	RoundBasedTimeModel timeModel(timerIndex, interval, startTime);

	ASSERT_EQUAL(1.5, timeModel.getTimeSpan(processState));

	processState->setGlobalAttribute(timerIndex, 1.3);
	ASSERT_EQUAL(0.2, timeModel.getTimeSpan(processState));

	processState->setGlobalAttribute(timerIndex, 1.5);
	ASSERT_EQUAL(1.0, timeModel.getTimeSpan(processState));

	processState->setGlobalAttribute(timerIndex, 1.6);
	ASSERT_EQUAL(0.9, timeModel.getTimeSpan(processState));

	processState->setGlobalAttribute(timerIndex, 4.5);
	ASSERT_EQUAL(1.0, timeModel.getTimeSpan(processState));

}

cute::suite getTimeModelTests(){
	cute::suite s;

	s.push_back(CUTE(roundBasedTimeModelTest));


	return s;
}



#endif /* TIMEMODELTEST_H_ */
