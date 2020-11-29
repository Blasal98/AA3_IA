#pragma once


class PathSearchAlgorithm {
public:
	enum algorithmType { BREADTH_FIRST_SEARCH, DIJKSTRA, BEST_FIRST_SEARCH, A_ASTERISK, COUNT };
	virtual bool setPath(Agent* a, Grid* g, Vector2D targetCell) = 0;
	algorithmType type;
	
};