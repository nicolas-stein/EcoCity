#ifndef ROADSQUARE_H
#define ROADSQUARE_H

#include "gridsquare.h"

class RoadSquare : public GridSquare
{
public:
	RoadSquare(int posX, int posY, RoadType roadType, RessourceManager *ressourceManager);
	void updatePixmap(RoadSquare ***roadGrid);

	RoadType getRoadType() const;

	bool isConnectedBottom() const;
	bool isConnectedTop() const;
	bool isConnectedLeft() const;
	bool isConnectedRight() const;

	double getCost() const;

private :
	RoadType roadType;

	bool connectedTop = false;
	bool connectedBottom = false;
	bool connectedLeft = false;
	bool connectedRight = false;
};

#endif // ROADSQUARE_H
