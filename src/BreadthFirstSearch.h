#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "Scene.h"
#include "Agent.h"
#include "Seek.h"
#include "PathFollowing.h"
#include "Grid.h"
#include "PathSearchAlgorithm.h"

class BreadthFirstSearch : public PathSearchAlgorithm{
public:
	void setPath(Agent* a, Grid* g) {
		type = PathSearchAlgorithm::algorithmType::BREADTH_FIRST_SEARCH;
	}
private:


};
