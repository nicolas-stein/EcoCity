#include "zonebuilding.h"

ZoneBuilding::ZoneBuilding(int posX, int posY, int width, int height, ZoneType zoneType, RessourceManager *ressourceManager): Building(posX, posY, width, height, BuildingZone, ressourceManager), zoneType(zoneType)
{

}

void ZoneBuilding::addCoveringZone(ZoneSquare *zoneSquare)
{
	coveringZones.append(zoneSquare);
}

const QList<ZoneSquare *> &ZoneBuilding::getCoveringZones() const
{
	return coveringZones;
}

ZoneType ZoneBuilding::getZoneType() const
{
	return zoneType;
}
