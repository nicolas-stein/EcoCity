#include "building.h"

Building::Building(int posX, int posY, int width, int height, Buildings::Type buildingType, RessourceManager *ressourceManager): posX(posX), posY(posY), buildingType(buildingType), ressourceManager(ressourceManager), width(width), height(height)
{
	pixmapItem.setPos(posX-BUILDING_MARGIN/2, posY-BUILDING_MARGIN/2);
	pixmapItem.setZValue(10);
	pixmapItem.setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

CustomGraphicsPixmapItem *Building::getPixmapItem()
{
	return &pixmapItem;
}

Buildings::Type Building::getBuildingType() const
{
	return buildingType;
}

void Building::setPos(int newPosX, int newPosY)
{
	posX = newPosX;
	posY = newPosY;
	pixmapItem.setPos(posX-BUILDING_MARGIN/2, posY-BUILDING_MARGIN/2);
}

int Building::getPosX() const
{
	return posX;
}

int Building::getPosY() const
{
	return posY;
}

int Building::getWidth() const
{
	return width;
}


int Building::getHeight() const
{
	return height;
}

bool Building::isConnectedToPower()
{
	return connectedToPower;
}
