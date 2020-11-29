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

struct Heuristed {
	Vector2D cell;
	float heuristic;

	bool operator() (const Heuristed& a, const Heuristed& b) {
		if (a.heuristic > b.heuristic) return true;
		else return false;
	}
};

class BestFirstSearch : public PathSearchAlgorithm {
public:
	BestFirstSearch() {
		type = PathSearchAlgorithm::algorithmType::BEST_FIRST_SEARCH;
	}
	bool setPath(Agent* a, Grid* g, Vector2D targetCell) {
		std::priority_queue<Heuristed, std::vector<Heuristed>, Heuristed> frontier;  //Create Frontier
		frontier.push(Heuristed{ g->pix2cell(a->getPosition()) ,0 });	//Push Start Cell to Frontier

		std::vector<std::pair<Vector2D, Vector2D>> came_from; //Creem vector de pair Vector2/Vector2
		came_from.push_back(std::make_pair(frontier.top().cell, Vector2D(-1, -1))); //Asignem que el node inicial ve de cap lloc

		std::cout << "Mida de la Frontera a cada Iteracio: " << std::endl;
		int auxCount = 0;
		while (!frontier.empty()) { //Mentre frontera no estigui buida

			Heuristed current = frontier.top(); //agafem primer element de frontier i li diem current
			frontier.pop(); //fem pop(pop_top) de la frontier
			//std::cout << current.x << " " << current.y << std::endl;
			if (current.cell == targetCell) break; //si l'hem trobat s'acaba

			std::vector<std::pair<Vector2D, bool>> neighbours = g->getNeighbours(current.cell); //agafem veins de Current
			for (int i = 0; i < neighbours.size(); i++) { //per cada vei de Current

				bool inCameFrom = false;

				for (int j = 0; j < came_from.size(); j++) { //per cada element de came_from 

					if (came_from[j].first == neighbours[i].first) {  //si el vei esta a came_from.first el marca com que ya esta
						inCameFrom = true;
						break;
					}
				}
				float heuristic = std::sqrtf(std::powf(neighbours[i].first.x - targetCell.x, 2) + std::powf(neighbours[i].first.y - targetCell.y, 2));
				if (!inCameFrom) {
					frontier.push(Heuristed{ neighbours[i].first, heuristic }); //sino estava doncs el pusheja a frontier ia came_from
					came_from.push_back(std::make_pair(neighbours[i].first, current.cell));

				}

			}
			//std::cout << frontier.size() << ",";
			auxCount++;

		}
		float auxPathLength = 0;
		std::vector<Vector2D> _path;
		_path.push_back(targetCell);
		while (_path[_path.size() - 1] != Vector2D(-1, -1)) {
			for (int i = 0; i < came_from.size(); i++) {
				if (came_from[i].first == _path[_path.size() - 1]) {
					_path.push_back(came_from[i].second);
					if (came_from[i].first.x == came_from[i].second.x + 1 && came_from[i].first.y == came_from[i].second.y + 1) { auxPathLength += std::sqrtf(2); }
					else if (came_from[i].first.x == came_from[i].second.x + 1 && came_from[i].first.y == came_from[i].second.y - 1) { auxPathLength += std::sqrtf(2); }
					else if (came_from[i].first.x == came_from[i].second.x - 1 && came_from[i].first.y == came_from[i].second.y + 1) { auxPathLength += std::sqrtf(2); }
					else if (came_from[i].first.x == came_from[i].second.x - 1 && came_from[i].first.y == came_from[i].second.y - 1) { auxPathLength += std::sqrtf(2); }
					else { auxPathLength++; }

					break;
				}
			}
		}
		for (int i = _path.size() - 2; i >= 0; i--) {
			a->addPathPoint(g->cell2pix(_path[i]));
		}
		std::cout << "Total de Iteracions: " << auxCount << " || Longitud del camí: " << auxPathLength << std::endl;
		return true;
	}
};