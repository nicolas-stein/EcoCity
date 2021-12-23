#ifndef GRIDSQUARE_H
#define GRIDSQUARE_H

#include "Constants.h"
#include <QGraphicsPixmapItem>

class GridSquare
{

public:
	GridSquare(GridType gridType, int posX, int posY, int width, int height);

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
};

#endif // GRIDSQUARE_H
