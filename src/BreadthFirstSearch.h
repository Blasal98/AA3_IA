#pragma once
#include <queue>
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
	BreadthFirstSearch() {
		type = PathSearchAlgorithm::algorithmType::BREADTH_FIRST_SEARCH;
	}
	bool setPath(Agent* a, Grid* g, Vector2D targetCell, bool _showAll) {
		std::queue<Vector2D> frontier;  //Create Frontier
		frontier.push(g->pix2cell(a->getPosition()));	//Push Start Cell to Frontier
		
		std::vector<std::pair<Vector2D,Vector2D>> came_from; //Creem vector de pair Vector2/Vector2
		came_from.push_back(std::make_pair(frontier.front(), Vector2D(-1, -1))); //Asignem que el node inicial ve de cap lloc

		//std::cout << "Mida de la Frontera a cada Iteracio: " << std::endl;
		int auxCount = 0;
		while (!frontier.empty()) { //Mentre frontera no estigui buida

			Vector2D current = frontier.front(); //agafem primer element de frontier i li diem current
			frontier.pop(); //fem pop(pop_front) de la frontier
			//std::cout << current.x << " " << current.y << std::endl;
			if (current == targetCell) break; //si l'hem trobat s'acaba

			std::vector<Vector2D> neighbours = g->getNeighbours(current); //agafem veins de Current
			for (int i = 0; i < neighbours.size(); i++) { //per cada vei de Current
				bool inCameFrom = false;
				for (int j = 0; j < came_from.size(); j++) { //per cada element de came_from

					if (came_from[j].first == neighbours[i])  //si el vei esta a came_from.first el marca com que ya esta
						inCameFrom = true;
				}
				if (!inCameFrom) {
					frontier.push(neighbours[i]); //sino estava doncs el pusheja a frontier ia came_from
					came_from.push_back(std::make_pair(neighbours[i],current));
					if(_showAll) a->addPathPoint(g->cell2pix(neighbours[i]));
				}
			}
			//std::cout << frontier.size() << ",";
			auxCount++;
		}
		if (!_showAll) {
			int auxPathLength = 0;
			std::vector<Vector2D> _path;
			_path.push_back(targetCell);
			while (_path[_path.size() - 1] != Vector2D(-1, -1)) {
				for (int i = 0; i < came_from.size(); i++) {
					if (came_from[i].first == _path[_path.size() - 1]) {
						_path.push_back(came_from[i].second);
						auxPathLength++;
						break;
					}
				}
			}
			for (int i = _path.size() - 2; i >= 0; i--) {
				a->addPathPoint(g->cell2pix(_path[i]));
			}
			std::cout << "Total de Iteracions: " << auxCount << " || Longitud del camí: " << auxPathLength << std::endl;
		}
		
		return true;
	}
private:


};
