#include "Grid.h"

using namespace std;

Grid::Grid(char* filename)
{
	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;

	// Initialize the terrain matrix from file (for each cell a zero value indicates it's a wall, positive values indicate terrain cell cost)
	std::ifstream infile(filename);
	std::string line;
	while (std::getline(infile, line))
	{
		vector<int> terrain_row;
		std::stringstream lineStream(line);
		std::string cell;
		while (std::getline(lineStream, cell, ','))
			terrain_row.push_back(atoi(cell.c_str()));
		SDL_assert(terrain_row.size() == num_cell_x);
		terrain.push_back(terrain_row);
	}
	SDL_assert(terrain.size() == num_cell_y);
	infile.close();
}

Grid::~Grid()
{
}

int Grid::getNumCellX()
{
	return num_cell_x;
}

int Grid::getNumCellY()
{
	return num_cell_y;
}

Vector2D Grid::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D Grid::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x / CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool Grid::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.y >= terrain.size()) || (cell.x >= terrain[0].size()))
		return false;
	return !(terrain[(unsigned int)cell.y][(unsigned int)cell.x] == 0);
}

std::vector<std::pair<Vector2D, bool>> Grid::getNeighbours(Vector2D cell) {
	std::vector<std::pair<Vector2D, bool>> returnVector;

	if (isValidCell(Vector2D(cell.x + 1, cell.y)))
		returnVector.push_back(std::make_pair(Vector2D(cell.x + 1, cell.y), false));
	if (isValidCell(Vector2D(cell.x - 1, cell.y)))
		returnVector.push_back(std::make_pair(Vector2D(cell.x - 1, cell.y), false));
	if (isValidCell(Vector2D(cell.x, cell.y + 1)))
		returnVector.push_back(std::make_pair(Vector2D(cell.x, cell.y + 1), false));
	if (isValidCell(Vector2D(cell.x, cell.y - 1)))
		returnVector.push_back(std::make_pair(Vector2D(cell.x, cell.y - 1), false));

	if (isValidCell(Vector2D(cell.x + 1, cell.y + 1)))
		returnVector.push_back(std::make_pair(Vector2D(cell.x + 1, cell.y + 1), true));
	if (isValidCell(Vector2D(cell.x - 1, cell.y - 1)))
		returnVector.push_back(std::make_pair(Vector2D(cell.x - 1, cell.y - 1), true));
	if (isValidCell(Vector2D(cell.x + 1, cell.y - 1)))
		returnVector.push_back(std::make_pair(Vector2D(cell.x + 1, cell.y - 1), true));
	if (isValidCell(Vector2D(cell.x - 1, cell.y + 1)))
		returnVector.push_back(std::make_pair(Vector2D(cell.x - 1, cell.y + 1), true));

	return returnVector;
}