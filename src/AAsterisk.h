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

struct WeightedHeuristed {
	Vector2D cell;
	float weight;

	bool operator() (const WeightedHeuristed& a, const WeightedHeuristed& b) {
		if (a.weight > b.weight) return true;
		else return false;
	}
};

class AAsterisk : public PathSearchAlgorithm {
public:
	AAsterisk() {
		type = PathSearchAlgorithm::algorithmType::A_ASTERISK;
	}
	bool setPath(Agent* a, Grid* g, Vector2D targetCell) {
		std::priority_queue<WeightedHeuristed, std::vector<WeightedHeuristed>, WeightedHeuristed> frontier;  //Create Frontier
		frontier.push(WeightedHeuristed{ g->pix2cell(a->getPosition()) ,0 });	//Push Start Cell to Frontier

		std::vector<std::pair<Vector2D, Vector2D>> came_from; //Creem vector de pair Vector2/Vector2
		came_from.push_back(std::make_pair(frontier.top().cell, Vector2D(-1, -1))); //Asignem que el node inicial ve de cap lloc

		std::vector<std::pair<Vector2D, int>> cost_so_far;
		cost_so_far.push_back(std::make_pair(frontier.top().cell, 0));

		std::cout << "Mida de la Frontera a cada Iteracio: " << std::endl;
		int auxCount = 0;
		while (!frontier.empty()) { //Mentre frontera no estigui buida

			WeightedHeuristed current = frontier.top(); //agafem primer element de frontier i li diem current
			frontier.pop(); //fem pop(pop_top) de la frontier
			//std::cout << current.x << " " << current.y << std::endl;
			if (current.cell == targetCell) break; //si l'hem trobat s'acaba

			std::vector<std::pair<Vector2D, bool>> neighbours = g->getNeighbours(current.cell); //agafem veins de Current
			for (int i = 0; i < neighbours.size(); i++) { //per cada vei de Current

				bool inCameFrom = false;
				bool newLowerCost = false;

				int auxCost = -1;

				for (int j = 0; j < came_from.size(); j++) { //per cada element de came_from 

					if (came_from[j].first == neighbours[i].first) {  //si el vei esta a came_from.first el marca com que ya esta
						inCameFrom = true;
						break;
					}
				}
				int changeIndex = -1;
				if (inCameFrom) { //si esta a camefrom
					for (int j = 0; j < cost_so_far.size(); j++) { //recorrem cost so far per trobar el seu cost
						if (cost_so_far[j].first == neighbours[i].first) {
							auxCost = cost_so_far[j].second;
							changeIndex = j;
							break;
						}
					}
				}
				float heuristic = std::sqrtf(std::powf(neighbours[i].first.x - targetCell.x, 2) + std::powf(neighbours[i].first.y - targetCell.y, 2));

				float newCost = -1;
				if (neighbours[i].second) newCost = current.weight + std::sqrtf(2);
				else newCost = current.weight + 1;

				if (auxCost > 0 && newCost < auxCost)
					newLowerCost = true; //suposant que totes les arestes sumin 1

				if (!inCameFrom) {
					frontier.push(WeightedHeuristed{ neighbours[i].first, newCost + heuristic }); //sino estava doncs el pusheja a frontier ia came_from
					came_from.push_back(std::make_pair(neighbours[i].first, current.cell));
					cost_so_far.push_back(std::make_pair(neighbours[i].first, newCost));

				}
				else if (newLowerCost) {
					frontier.push(WeightedHeuristed{ neighbours[i].first, newCost + heuristic }); //sino estava doncs el pusheja a frontier ia came_from
					came_from[changeIndex] = std::make_pair(neighbours[i].first, current.cell);
					cost_so_far[changeIndex] = std::make_pair(neighbours[i].first, newCost);

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