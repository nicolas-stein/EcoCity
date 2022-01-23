#ifndef GRIDSQUARE_H
#define GRIDSQUARE_H

#include "constants.h"
#include "Game/ressourcemanager.h"
#include "customgraphicspixmapitem.h"

class GridSquare
{

public:
	GridSquare(Grid::Type gridType, int posX, int posY, int width, int height, RessourceManager *ressourceManager);
	virtual ~GridSquare(){};

	CustomGraphicsPixmapItem *getPixmapItem();
	void setPos(int posX, int newPosY);

	Grid::Type getGridType() const;

	int getPosX() const;
	int getPosY() const;

private:
	Grid::Type gridType;
protected :
	CustomGraphicsPixmapItem pixmapItem;
	int posX, posY, width, height;
	RessourceManager *ressourceManager;
};

#endif // GRIDSQUARE_H
