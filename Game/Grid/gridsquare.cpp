#include "gridsquare.h"

GridSquare::GridSquare(GridType gridType, int posX, int posY, int width, int height, RessourceManager *ressourceManager) : gridType(gridType), posX(posX), posY(posY), width(width), height(height), ressourceManager(ressourceManager)
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
	if(gridType==GridTerrain){
		pixmapItem.setPos(newPosX-TERRAIN_SQUARE_MARGIN/2, newPosY-TERRAIN_SQUARE_MARGIN/2);
	}
	else if(gridType==GridRoad){
		pixmapItem.setPos(newPosX-ROAD_SQUARE_MARGIN/2, newPosY-ROAD_SQUARE_MARGIN/2);
	}
	else if(gridType==GridZone){
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
