#include "building.h"

Building::Building(int posX, int posY, int width, int height, BuildingType buildingType, RessourceManager *ressourceManager): posX(posX), posY(posY), buildingType(buildingType), ressourceManager(ressourceManager), width(width), height(height)
{
	pixmapItem.setPos(posX-BUILDING_MARGIN/2, posY-BUILDING_MARGIN/2);
	pixmapItem.setZValue(10);
	pixmapItem.setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QGraphicsPixmapItem *Building::getPixmapItem()
{
	return &pixmapItem;
}

BuildingType Building::getBuildingType() const
{
	return buildingType;
}

void Building::setPos(int newPosX, int newPosY)
{
	posX = newPosX;
	posY = newPosY;
	pixmapItem.setPos(newPosX, newPosY);
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
