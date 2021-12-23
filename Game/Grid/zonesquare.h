#ifndef ZONESQUARE_H
#define ZONESQUARE_H

#include "gridsquare.h"
#include "roadsquare.h"
#include "Game/Building/residentialbuilding.h"
#include "Constants.h"

class ZoneSquare : public GridSquare
{
public:
	ZoneSquare(int posX, int posY, ZoneType zoneType, QMap<ZoneType, QPixmap> *zonePixmaps);
	void setZoneType(ZoneType zoneType);
	ZoneType getZoneType() const;

	int isZoneAdjacentToRoad(RoadSquare ***roadGrid);
	bool isZoneValid(ZoneSquare ***zoneGrid, RoadSquare ***roadGrid);

	Building *getBuilding();
	void setBuilding(Building *newBuilding);

private:
	ZoneType zoneType;
	QMap<ZoneType, QPixmap> *zonePixmaps;
	Building *building = nullptr;
};

#endif // ZONESQUARE_H
