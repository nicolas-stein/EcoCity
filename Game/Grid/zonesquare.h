#ifndef ZONESQUARE_H
#define ZONESQUARE_H

#include "gridsquare.h"
#include "Game/Building/ZoneBuilding/zonebuilding.h"

class RoadSquare;

class ZoneSquare : public GridSquare
{
public:
	ZoneSquare(int posX, int posY, Grid::Zone::Type zoneType, RessourceManager *ressourceManager);
	void setZoneType(Grid::Zone::Type zoneType);
	Grid::Zone::Type getZoneType() const;

	int isZoneAdjacentToRoad(RoadSquare ***roadGrid);
	bool isZoneValid(ZoneSquare ***zoneGrid, RoadSquare ***roadGrid);

	ZoneBuilding *getBuilding();
	void setBuilding(ZoneBuilding *newBuilding);

private:
	Grid::Zone::Type zoneType;
	ZoneBuilding *building = nullptr;
};

#endif // ZONESQUARE_H
