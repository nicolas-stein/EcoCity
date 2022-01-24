#ifndef ROADSQUARE_H
#define ROADSQUARE_H

#include "gridsquare.h"

//Classe représentant un carré de route
class RoadSquare : public GridSquare
{
public:
	RoadSquare(int posX, int posY, Grid::Road::Type roadType, RessourceManager *ressourceManager);

	void updatePixmap(RoadSquare ***roadGrid);	//Mise à jour de la texture de la route

	Grid::Road::Type getRoadType() const;

	bool isConnectedBottom() const;
	bool isConnectedTop() const;
	bool isConnectedLeft() const;
	bool isConnectedRight() const;

	double getCost() const;						//Cout de construction de la route

private :
	Grid::Road::Type roadType;					//Type de route

	bool connectedTop = false;					//La route est-elle connectée à une autre route en dessous ?
	bool connectedBottom = false;				//La route est-elle connectée à une autre route au dessus ?
	bool connectedLeft = false;					//La route est-elle connectée à une autre route à gauche ?
	bool connectedRight = false;				//La route est-elle connectée à une autre route à droite ?
};

#endif // ROADSQUARE_H
