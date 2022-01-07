#ifndef GRIDSQUARE_H
#define GRIDSQUARE_H

#include "constants.h"
#include "Game/ressourcemanager.h"

#include <QGraphicsPixmapItem>

class GridSquare
{

public:
	GridSquare(GridType gridType, int posX, int posY, int width, int height, RessourceManager *ressourceManager);

	QGraphicsPixmapItem *getPixmapItem();
	void setPos(int posX, int newPosY);

	GridType getGridType() const;

	int getPosX() const;
	int getPosY() const;

private:
	GridType gridType;
protected :
	QGraphicsPixmapItem pixmapItem;
	int posX, posY, width, height;
	RessourceManager *ressourceManager;
};

#endif // GRIDSQUARE_H
