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

class BestFirstSearch : public PathSearchAlgorithm {
public:
	BestFirstSearch() {
		type = PathSearchAlgorithm::algorithmType::BEST_FIRST_SEARCH;
	}
	void setPath(Agent* a, Grid* g) {

	}
};