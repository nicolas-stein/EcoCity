#ifndef BUILDING_H
#define BUILDING_H

#include "constants.h"
#include "Game/ressourcemanager.h"

#include <QGraphicsPixmapItem>

class Building
{
public:
	Building(int posX, int posY, int width, int height, BuildingType buildingType, RessourceManager *ressourceManager);

	QGraphicsPixmapItem *getPixmapItem();

	BuildingType getBuildingType() const;

	void setPos(int newPosX, int newPosY);
	int getPosX() const;
	int getPosY() const;

	int getWidth() const;
	int getHeight() const;

private :
	int posX;
	int posY;
	BuildingType buildingType;
	RessourceManager *ressourceManager;


protected :
	QGraphicsPixmapItem pixmapItem;
	int width;
	int height;
};

#endif // BUILDING_H
