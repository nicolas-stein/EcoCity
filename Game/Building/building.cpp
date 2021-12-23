#include "building.h"

Building::Building(int posX, int posY, int width, int height, ZoneType buildingType, QMap<ZoneType, QList<QPixmap>> *zonePixmaps): posX(posX), posY(posY), width(width), height(height), buildingType(buildingType), zonePixmaps(zonePixmaps)
{
	pixmapItem.setPos(posX-BUILDING_MARGIN/2, posY-BUILDING_MARGIN/2);
}

void Building::addCoveringZone(ZoneSquare *zoneSquare)
{
	coveringZones.append(zoneSquare);
}

QGraphicsPixmapItem *Building::getPixmapItem()
{
	return &pixmapItem;
}

const QList<ZoneSquare *> &Building::getCoveringZones() const
{
	return coveringZones;
}
