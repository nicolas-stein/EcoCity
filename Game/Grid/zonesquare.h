#ifndef ZONESQUARE_H
#define ZONESQUARE_H

#include "gridsquare.h"
#include "Game/Building/ZoneBuilding/zonebuilding.h"

class RoadSquare;

class ZoneSquare : public GridSquare
{
public:
	ZoneSquare(int posX, int posY, ZoneType zoneType, RessourceManager *ressourceManager);
	void setZoneType(ZoneType zoneType);
	ZoneType getZoneType() const;

	int isZoneAdjacentToRoad(RoadSquare ***roadGrid);
	bool isZoneValid(ZoneSquare ***zoneGrid, RoadSquare ***roadGrid);

	ZoneBuilding *getBuilding();
	void setBuilding(ZoneBuilding *newBuilding);

private:
	ZoneType zoneType;
	ZoneBuilding *building = nullptr;
};

#endif // ZONESQUARE_H
