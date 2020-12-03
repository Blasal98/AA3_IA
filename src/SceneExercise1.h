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
#include "BreadthFirstSearch.h"
#include "Dijkstra.h"
#include "BestFirstSearch.h"
#include "AAsterisk.h"
#include "PathSearchAlgorithm.h"


class SceneExercise1 :
	public Scene
{
public:
	SceneExercise1();
	~SceneExercise1();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();
	
	void nextAlgorithm();
	void previousAlgorithm();
	void createNewAlgorithm(PathSearchAlgorithm::algorithmType _t);
	void Instances20();


private:
	bool pause;
	bool showAll;
	std::vector<Agent*> agents;
	Vector2D coinPosition;

	Grid *maze;
	bool draw_grid;
		
	void drawMaze();
	void drawCoin();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	bool loadTextures(char* filename_bg, char* filename_coin);

	
	PathSearchAlgorithm* currentAlgorithm;
	bool pathSetted;

	void setRandPositions();
	int instance;
	std::vector<std::pair<Vector2D,Vector2D>> instances;

};
