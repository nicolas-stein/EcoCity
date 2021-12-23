#include "gridsquare.h"

GridSquare::GridSquare(GridType gridType, int posX, int posY, int width, int height) : gridType(gridType), posX(posX), posY(posY), width(width), height(height)
{

}

QGraphicsPixmapItem *GridSquare::getPixmapItem()
{
	return &pixmapItem;
}

void GridSquare::setPos(int newPosX, int newPosY)
{
	posX = newPosX;
	posY = newPosY;
	if(gridType==Terrain){
		pixmapItem.setPos(newPosX-TERRAIN_SQUARE_MARGIN/2, newPosY-TERRAIN_SQUARE_MARGIN/2);
	}
	else if(gridType==Road){
		pixmapItem.setPos(newPosX-ROAD_SQUARE_MARGIN/2, newPosY-ROAD_SQUARE_MARGIN/2);
	}
	else if(gridType==Zone){
		pixmapItem.setPos(newPosX-ZONE_SQUARE_MARGIN/2, newPosY-ZONE_SQUARE_MARGIN/2);
	}
}

GridType GridSquare::getGridType() const
{
	return gridType;
}

int GridSquare::getPosX() const
{
	return posX;
}

int GridSquare::getPosY() const
{
	return posY;
}
