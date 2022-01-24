#include "gridsquare.h"

GridSquare::GridSquare(Grid::Type gridType, int posX, int posY, int width, int height, RessourceManager *ressourceManager) : gridType(gridType), posX(posX), posY(posY), width(width), height(height), ressourceManager(ressourceManager)
{

}

Grid::Type GridSquare::getGridType() const
{
	return gridType;
}

CustomGraphicsPixmapItem *GridSquare::getPixmapItem()
{
	return &pixmapItem;
}

int GridSquare::getPosX() const
{
	return posX;
}

int GridSquare::getPosY() const
{
	return posY;
}

void GridSquare::setPos(int newPosX, int newPosY)
{
	posX = newPosX;
	posY = newPosY;
	//Change la position du pixmapItem avec une marge différente en fonction du type de case
	if(gridType==Grid::Type::GridTerrain){
		pixmapItem.setPos(newPosX-TERRAIN_SQUARE_MARGIN/2, newPosY-TERRAIN_SQUARE_MARGIN/2);
	}
	else if(gridType==Grid::Type::GridRoad){
		pixmapItem.setPos(newPosX-ROAD_SQUARE_MARGIN/2, newPosY-ROAD_SQUARE_MARGIN/2);
	}
	else if(gridType==Grid::Type::GridZone){
		pixmapItem.setPos(newPosX-ZONE_SQUARE_MARGIN/2, newPosY-ZONE_SQUARE_MARGIN/2);
	}
}
