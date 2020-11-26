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

class AAsterisk : public PathSearchAlgorithm {
public:
	AAsterisk() {
		type = PathSearchAlgorithm::algorithmType::A_ASTERISK;
	}
	void setPath(Agent* a, Grid* g) {

	}
};