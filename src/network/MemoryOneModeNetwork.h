/*
 * MemoryOneModeNetwork.h
 *
 *  Created on: Nov 20, 2012
 *      Author: cws
 */

#ifndef MEMORYONEMODENETWORK_H_
#define MEMORYONEMODENETWORK_H_

#include "OneModeNetwork.h"

#include <map>
#include <set>
#include <algorithm> // for set_intersection
//#include <boost/unordered_map.hpp>
//#include <boost/bimap.hpp> // bi-directional linking of elements
//#include <boost/bimap/set_of.hpp>
//#include <boost/bimap/multiset_of.hpp>


// forward declaration
class MemoryOneModeNetworkMemento;

/**
 * Extension of OneModeNetwork with a number of look ups.
 * Updates of a MemoryOneModeNetwork are slower than updates in a OneModeNetwork.
 * However, the lookup of certain values is much faster.
 * The graph is internally represented both as matrix and a list of pointers to edge vales
 */
class MemoryOneModeNetwork : public Network {

	friend class NetworkUtils;

public:

	MemoryOneModeNetwork(int size, bool directed=true, bool reflexive = false);
	MemoryOneModeNetwork(std::vector<std::vector<double> > graph, bool directed=true,
			bool reflexive = false);
	~MemoryOneModeNetwork();

	/**
	 * add tie to network
	 */
	bool addTie(int i, int j);

	/**
	 * set tie to value
	 */
	bool setTie(int i, int j, double value);

	/**
	 * remove tie
	 */
	bool removeTie(int i, int j);

	/**
	 * Directed networks can have directed ties i -> j without
	 * the existence if a tie j -> i
	 */
	bool isDirected() const;

	/**
	 * Reflexive networks can have reflexive ties i -> i
	 */
	bool isReflexive() const;

	/*
	 * Get size of the network which equals the number of actors
	 */
	int getSize();

	/**
	 * Returns whether the tie value is greater than zero.
	 * Overrides graph lookup in OneModeNetwork
	 */
	bool hasTie(int i, int j);

	/**
	 * Get value of tie from i to j
	 * Overrides graph lookup in OneModeNetwork
	 * Independent of the real value this always returns 1.0
	 * if the tie has a positive value and 0 otherwise
	 */
	double getTieValue(int i, int j);


	/**
	 * Returns the (dichotomized) in-degree of a node i in O(1)
	 */
	int getInDegree(int i);

	/**
	 * Returns the (dichotomized) out-degree of a node i in O(1)
	 */
	int getOutDegree(int i);

	/**
	 * Returns a set of outgoing neighbors of node i,
	 * indicating all nodes that i has a tie to
	 */
	std::set<int> getOutgoingNeighbors(int i);

	/**
	 * Returns a set of incoming neighbors of node i,
	 * indicating all nodes that have a tie to node i
	 */
	std::set<int> getIncomingNeighbors(int i);

	/**
	 * Returns a set of neigbors who are reciprocally connected
	 */
	std::set<int> getReciprocalNeighbors(int i);

	/**
	 * returns nodes k that are connected with nodes i and j:
	 * i -> {k} -> j
	 */
	std::set<int> getIntermediateNodesForward(int i, int j);

	/**
	 * returns nodes k that are connected with nodes i and j:
	 * i <- {k} <- j
	 */
	std::set<int> getIntermediateNodesBackward(int i, int j);

	/**
	 * returns nodes k that are connected with nodes i and j:
	 * i -> {k} <- j
	 */
	std::set<int> getIntermediateNodesUp(int i, int j);

	/**
	 * returns nodes k that are connected with nodes i and j:
	 * i <- {k} -> j
	 */
	std::set<int> getIntermediateNodesDown(int i, int j);

	/**
	 * returns all nodes k that are connected with nodes i in distance 2:
	 * i -> ? -> k
	 */
	std::set<int> getNodesInDistanceTwo(int i);

	/**
	 * Saves the current proess state in a memento and returns it
	 */
	MemoryOneModeNetworkMemento * saveToMemento();

	/**
	 * Restores the process state from memento
	 */
	void restoreFromMemento(NetworkMemento * memento);

	/**
	 * Accessor 2d vector representation of the network
	 */
	std::vector<std::vector<double> > getGraph();

	/**
	 * Add actor with ID id to the network. It has in- and out-degree of zero
	 */
	void addActor(size_t id);

	/**
	 * delete actor with given id
	 */
	void deleteActor(size_t id);

	/**
	 * get actor IDs
	 * TODO: copies the set - would a pointer be better?
	 */
	const std::set<int> getActorIDs();


private:

	// lookup maps
	std::map<int, int> _inDegreeMap;
	std::map<int, int> _outDegreeMap;

	std::map<int,std::set<int>* > _outgoingNeighborsMap;
	std::map<int,std::set<int>* > _incomingNeighborsMap;

	std::set<int> _actorIDs;

protected:

	bool _reflexive;
	bool _directed;
	int _size;


	void init(int size, bool directed, bool reflexive);
	void initActor(int id);
	void updateInternalRepresentation(int i, int j, double oldValue, double newValue);


	/*
	 * checks whether an index i is in the range [0, size-1]
	 * TODO: is this deprecated as IDs can be anything?
	 */
	bool isIndexValid(int i);


	std::set<int> intersectSets(std::set<int>* set1, std::set<int>* set2);

};

class MemoryOneModeNetworkMemento : public OneModeNetworkMemento{

private:

	MemoryOneModeNetworkMemento();

	std::map<int, int> getSavedInDegreeMap();
	std::map<int, int> getSavedOutDegreeMap();
	std::map<int, std::set<int>* > getSavedIncomingNeighborsMap(std::map<int, std::set<int>* > source);
	std::map<int, std::set<int>* > getSavedOutgoingNeighborsMap(std::map<int, std::set<int>* > source);

	void saveInDegreeMap(const std::map<int, int> inDegreeMap);
	void saveOutDegreeMap(const std::map<int, int> outDegreeMap);
	void saveIncomingNeighborsMap(const std::map<int, std::set<int>* > incomingNeighborsMap);
	void saveOutgoingNeighborsMap(const std::map<int, std::set<int>* > outgoingNeighborsMap);

	std::map<int,std::set<int>* > copyNeighborsSet(
			std::map<int,std::set<int>* > destination,
			std::map<int,std::set<int>* > source);

	std::map<int, int> _inDegreeMap;
	std::map<int, int> _outDegreeMap;
	std::map<int,std::set<int>* > _outgoingNeighborsMap;
	std::map<int,std::set<int>* > _incomingNeighborsMap;

	friend class MemoryOneModeNetwork;

};

class MockMemoryOneModeNetwork : public MemoryOneModeNetwork{

public:

	MockMemoryOneModeNetwork(int size, bool directed=true, bool reflexive = false) :
	MemoryOneModeNetwork(size, directed, reflexive){

	}

	MockMemoryOneModeNetwork(std::vector<std::vector<double> > graph, bool directed=true,
				bool reflexive = false) : MemoryOneModeNetwork(graph, directed, reflexive){

	}

protected:

	// do not make any internal updates after initialization
	void updateInternalRepresentation(int i, int j, double oldValue, double newValue){

	}
};


#endif /* MEMORYONEMODENETWORK_H_ */
