#ifndef ZONESQUARE_H
#define ZONESQUARE_H

#include "gridsquare.h"
#include "Game/Building/ZoneBuilding/zonebuilding.h"

class RoadSquare;

//Classe représentant un carré de zone
class ZoneSquare : public GridSquare
{
public:
	ZoneSquare(int posX, int posY, Grid::Zone::Type zoneType, RessourceManager *ressourceManager);
	void setZoneType(Grid::Zone::Type zoneType);
	Grid::Zone::Type getZoneType() const;

	int isZoneAdjacentToRoad(RoadSquare ***roadGrid);					//Est ce que la zone est directement collée à une route
	bool isZoneValid(ZoneSquare ***zoneGrid, RoadSquare ***roadGrid);	//Est ce que la zone est valide ?

	ZoneBuilding *getBuilding();
	void setBuilding(ZoneBuilding *newBuilding);

private:
	Grid::Zone::Type zoneType;			//Type de zone (résidentielle, commerciale, industrielle)
	ZoneBuilding *building = nullptr;	//Batiment posé sur la zone
};

#endif // ZONESQUARE_H
