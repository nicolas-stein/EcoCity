#ifndef ZONEBUILDING_H
#define ZONEBUILDING_H

#include "../building.h"

class ZoneSquare;

class ZoneBuilding : public Building
{
public:
	ZoneBuilding(int posX, int posY, int width, int height, ZoneType zoneType, RessourceManager *ressourceManager);

	void addCoveringZone(ZoneSquare* zoneSquare);
	const QList<ZoneSquare *> &getCoveringZones() const;

	ZoneType getZoneType() const;

private :
	QList<ZoneSquare*> coveringZones;
	ZoneType zoneType;
};

#endif // ZONEBUILDING_H
