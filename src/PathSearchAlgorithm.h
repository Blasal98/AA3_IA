#pragma once


class PathSearchAlgorithm {
public:
	enum algorithmType { BREADTH_FIRST_SEARCH, DIJKSTRA, BEST_FIRST_SEARCH, A_ASTERISK, COUNT };
	virtual void setPath(Agent* a, Grid* g) = 0;
	algorithmType type;
	
};