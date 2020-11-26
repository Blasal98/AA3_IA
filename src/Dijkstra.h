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

class Dijkstra : public PathSearchAlgorithm {
public:
	Dijkstra(){
		type = PathSearchAlgorithm::algorithmType::DIJKSTRA;
	}
	void setPath(Agent* a, Grid* g) {

	}
};