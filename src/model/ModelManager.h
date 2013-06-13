/*
 * ModelManager.h
 *
 *  Created on: Nov 29, 2012
 *      Author: cws
 */

#ifndef MODELMANAGER_H_
#define MODELMANAGER_H_

#include <string>
//#include <boost/unordered_map.hpp>
#include "TimeModel.h"
#include "Updater.h"
#include "ChangeModel.h"
#include "Updater.h"

/**
 * manages different set of models and links these to one another:
 * * Each TimeModel can be linked to several ChangeModel's
 * * Each ChangeModel can be linked to several Updater's
 */
class ModelManager{

public:

	ModelManager();

	void addTimeUpdater(TimeUpdater * timeUpdater);

	void addTimeModel(TimeModel * timeModel);

	void addChangeModel(TimeModel * timeModel, ChangeModel * changeModel);

	void addUpdater(ChangeModel * changeModel, Updater * updater);

	std::vector<TimeModel*> * getTimeModels();

	/**
	 * Get all TimeUpdater objects.
	 * These are all time dependent network changes.
	 */
	std::vector<TimeUpdater*> * getTimeUpdater();

	/**
	 * Get the change model based on the index determined
	 * by a comparison of time models
	 */
	std::vector<ChangeModel*> * getChangeModels(TimeModel* timeModel);

	std::vector<ChangeModel*> * getChangeModels(std::set<TimeModel*> timeModel);

	/**
	 * Get the change updater based on the index determined
	 * by a comparison of time models
	 */
	std::vector<Updater*> * getUpdaters(ChangeModel* changeModel);

	std::string getLatestMessage();

private:

	std::vector<TimeModel*>_timeModels;
	std::vector<TimeUpdater*> _timeUpdaters;
	std::map<TimeModel*, std::vector<ChangeModel*> > _timeModelChangeModelMap;
	std::map<ChangeModel*, std::vector<Updater*> > _changeModelChangeUpdaterMap;

	// std::vector<ChangeModel*> _changeModels;
	// std::vector<Updater*> _updaters;
};


#endif /* MODELMANAGER_H_ */
