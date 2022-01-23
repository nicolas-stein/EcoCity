#ifndef BUILDING_H
#define BUILDING_H

#include "constants.h"
#include "Game/ressourcemanager.h"
#include "customgraphicspixmapitem.h"

#include <QPainter>

class Building
{
public:
	Building(int posX, int posY, int width, int height, Buildings::Type buildingType, RessourceManager *ressourceManager);
    virtual ~Building(){};

	CustomGraphicsPixmapItem *getPixmapItem();
	Buildings::Type getBuildingType() const;

	void setPos(int newPosX, int newPosY);
	int getPosX() const;
	int getPosY() const;
	int getWidth() const;
	int getHeight() const;

	bool isConnectedToPower();

	virtual void updatePixmap(bool showToolTips) = 0;
	virtual double getPollution() const = 0;
private :
	int posX;
	int posY;
	Buildings::Type buildingType;


protected :
	RessourceManager *ressourceManager;
	CustomGraphicsPixmapItem pixmapItem;
	int width;
	int height;
	bool connectedToPower = false;
};

#endif // BUILDING_H
