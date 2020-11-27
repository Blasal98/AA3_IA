#include "SceneExercise1.h"

using namespace std;

SceneExercise1::SceneExercise1()
{
	draw_grid = false;
	maze = new Grid("../res/maze.csv");

	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));

	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agent->setBehavior(new PathFollowing);
	agent->setTarget(Vector2D(-20,-20));
	agents.push_back(agent);

	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1,-1);
	while (!maze->isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
	agents[0]->setPosition(maze->cell2pix(rand_cell));
	//agents[0]->setPosition(maze->cell2pix(Vector2D(1,1)));

	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1,-1);
	while ((!maze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell)<3))
		coinPosition = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
	//coinPosition = Vector2D(38,22);

	
	currentAlgorithm = new BreadthFirstSearch();
	changeTitle = false;
	pathSetted = false;
}

SceneExercise1::~SceneExercise1()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneExercise1::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		break;
	case SDL_MOUSEMOTION:
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT) {
			nextAlgorithm();
			changeTitle = true;
			pathSetted = false;
		}
			
		else if (event->button.button == SDL_BUTTON_RIGHT) {
			previousAlgorithm();
			changeTitle = true;
			pathSetted = false;
		}
			
		break;
	default:
		break;
	}

	/*Vector2D cell = maze->pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));
	if (maze->isValidCell(cell)) {
		agents[0]->addPathPoint(maze->cell2pix(cell));
	}*/
	/*Vector2D cell = Vector2D(1, 2);
	if (maze->isValidCell(cell)) {
		agents[0]->addPathPoint(maze->cell2pix(cell));
	}*/

	if(!pathSetted){

		pathSetted = currentAlgorithm->setPath(agents[0], maze, coinPosition);
	}
	agents[0]->update(dtime, event);

	// if we have arrived to the coin, replace it in a random cell!
	if ((agents[0]->getCurrentTargetIndex() == -1) && (maze->pix2cell(agents[0]->getPosition()) == coinPosition))
	{
		coinPosition = Vector2D(-1, -1);
		while ((!maze->isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, maze->pix2cell(agents[0]->getPosition()))<3))
			coinPosition = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));

		pathSetted = false;
	}
	
}

void SceneExercise1::draw()
{
	drawMaze();
	drawCoin();

	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i+=CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	agents[0]->draw();
}

const char* SceneExercise1::getTitle()
{
	switch (currentAlgorithm->type)
	{
	case PathSearchAlgorithm::algorithmType::BREADTH_FIRST_SEARCH:
		return "Exercise1 :: BREADTH_FIRST_SEARCH";
		break;
	case PathSearchAlgorithm::algorithmType::DIJKSTRA:
		return "Exercise1 :: DIJKSTRA";
		break;
	case PathSearchAlgorithm::algorithmType::BEST_FIRST_SEARCH:
		return "Exercise1 :: BEST_FIRST_SEARCH";
		break;
	case PathSearchAlgorithm::algorithmType::A_ASTERISK:
		return "Exercise1 :: A_ASTERISK";
		break;
	default:
		return "Exercise1 :: NO ALGORITHM SELECTED";
		break;
	}
	return  "";
}

void SceneExercise1::drawMaze()
{
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
	SDL_Rect rect;
	Vector2D coords;
	for (int j = 0; j < maze->getNumCellY(); j++)
	{
		for (int i = 0; i < maze->getNumCellX(); i++)
		{		
			if (!maze->isValidCell(Vector2D((float)i, (float)j)))
			{
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
				coords = maze->cell2pix(Vector2D((float)i, (float)j)) - Vector2D((float)CELL_SIZE / 2, (float)CELL_SIZE / 2);
				rect = { (int)coords.x, (int)coords.y, CELL_SIZE, CELL_SIZE };
				SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &rect);
			} else {
				// Do not draw if it is not necessary (bg is already black)
			}
					
			
		}
	}
	//Alternative: render a backgroud texture:
	//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
}

void SceneExercise1::drawCoin()
{
	Vector2D coin_coords = maze->cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}


bool SceneExercise1::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface *image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

PathSearchAlgorithm* getNewAlgorithm(PathSearchAlgorithm::algorithmType _t) {
	switch (_t) {
	case PathSearchAlgorithm::algorithmType::BREADTH_FIRST_SEARCH:
		return new BreadthFirstSearch();
		break;
	case PathSearchAlgorithm::algorithmType::DIJKSTRA:
		return new Dijkstra();
		break;
	case PathSearchAlgorithm::algorithmType::BEST_FIRST_SEARCH:
		return new BestFirstSearch();
		break;
	case PathSearchAlgorithm::algorithmType::A_ASTERISK:
		return new AAsterisk();
		break;
	default:
		return NULL;
		break;
	}
}

void SceneExercise1::nextAlgorithm() {
	PathSearchAlgorithm::algorithmType auxType = currentAlgorithm->type;
	auxType = (PathSearchAlgorithm::algorithmType)((int)auxType + 1);
	if (auxType == PathSearchAlgorithm::algorithmType::COUNT) auxType = (PathSearchAlgorithm::algorithmType)0;
	delete(currentAlgorithm);
	currentAlgorithm = getNewAlgorithm(auxType);
	
}

void SceneExercise1::previousAlgorithm() {
	PathSearchAlgorithm::algorithmType auxType = currentAlgorithm->type;
	if (auxType == PathSearchAlgorithm::algorithmType::BREADTH_FIRST_SEARCH) auxType = (PathSearchAlgorithm::algorithmType)3;
	else auxType = (PathSearchAlgorithm::algorithmType)((int)auxType - 1);
	delete(currentAlgorithm);
	currentAlgorithm = getNewAlgorithm(auxType);
}


