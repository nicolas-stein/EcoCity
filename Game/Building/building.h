#ifndef BUILDING_H
#define BUILDING_H

#include "constants.h"
#include "Game/ressourcemanager.h"

#include <QGraphicsPixmapItem>
#include <QPainter>

class Building
{
public:
	Building(int posX, int posY, int width, int height, BuildingType buildingType, RessourceManager *ressourceManager);
    virtual ~Building(){};

	QGraphicsPixmapItem *getPixmapItem();

	BuildingType getBuildingType() const;

	void setPos(int newPosX, int newPosY);
	int getPosX() const;
	int getPosY() const;

	int getWidth() const;
	int getHeight() const;

	virtual void updatePixmap(bool showToolTips) = 0;

	bool isConnectedToPower();

private :
	int posX;
	int posY;
	BuildingType buildingType;


protected :
	RessourceManager *ressourceManager;
	QGraphicsPixmapItem pixmapItem;
	int width;
	int height;
	bool connectedToPower = false;
};

#endif // BUILDING_H
