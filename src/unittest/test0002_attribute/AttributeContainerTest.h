/*
 * AttributeContainerTest.h
 *
 *  Created on: Nov 26, 2012
 *      Author: cws
 */

#ifndef ATTRIBUTECONTAINTERTEST_H_
#define ATTRIBUTECONTAINTERTEST_H_

#include <vector>

AttributeContainer getTwoActorAttributeContainer(){
	int nActors = 2;
	double initialValue = 0.0;
	double min = 0.0;
	double max = 3.0;
	double by = 1.0;
	AttributeContainer a(nActors, initialValue);
	a.setMin(min);
	a.setMax(max);
	a.setBy(by);
	return a;
}

void attributeConstructorTest(){
	int nActors = 10;
	double initialValue = 3.0;
	double min = 0.0;
	double max = 5.0;
	double by = 1.0;

	// should throw invalid argument exception as initialValue is not in range
	// ASSERT_THROWS(AttributeContainer a1(nActors, initialValue, 0.0, 1.0, 1.0, 0.0),
	// 		std::invalid_argument);
	// ASSERT_THROWS(AttributeContainer a1(nActors, 1.0, 0.0, 2.0, 2.1),
	// 		std::invalid_argument);

	AttributeContainer a2(nActors, 0.5);
	ASSERT(a2.getSize() == nActors);

	AttributeContainer a3(nActors, initialValue);
	a3.setMin(min);
	a3.setMax(max);
	a3.setBy(by);

}

void setAndRetreiveAttributes(){
	AttributeContainer ac = getTwoActorAttributeContainer();

	ASSERT(ac.getValue(0) == 0);
	ac.increaseValue(0);
	ASSERT(ac.getValue(0) == 1);
	ac.increaseValue(0);
	ASSERT(ac.increaseValue(0));
	ASSERT(ac.getValue(0) == 3);
	ASSERT(!ac.increaseValue(0));
	ac.increaseValue(0);
	ASSERT(ac.getValue(0) == 3);
	ac.decreaseValue(0);
	ac.decreaseValue(0);
	ASSERT(ac.decreaseValue(0));
	ASSERT(!ac.decreaseValue(0));

	ASSERT(ac.setValue(1,3.0));
	ASSERT(ac.isMax(1));
	ASSERT(!ac.isMin(1));
	ac.setValue(1,0.0);
	ASSERT(ac.isMin(1));

}


cute::suite getTestAttributeContainerSuite(){
	cute::suite s;

	s.push_back(CUTE(attributeConstructorTest));
	s.push_back(CUTE(setAndRetreiveAttributes));

	return s;
}


#endif /* ATTRIBUTECONTAINTERTEST_H_ */
