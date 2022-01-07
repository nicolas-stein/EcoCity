#include "terrainsquare.h"

TerrainSquare::TerrainSquare(double noise, int posX, int posY, RessourceManager *ressourceManager) : GridSquare(GridTerrain, posX, posY, TERRAIN_SQUARE_SIZE, TERRAIN_SQUARE_SIZE, ressourceManager)
{
	if(noise < 0.25){
		terrainType = Water;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Water));
	}
	else if(noise < 0.30){
		terrainType = Sand;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Sand));
	}
	else if(noise < 0.77){
		terrainType = Grass;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Grass));
	}
	else if(noise < 0.80){
		terrainType = Rock;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Rock));
	}
	else{
		terrainType = Mountain;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Mountain));
	}
	pixmapItem.setPos(posX-TERRAIN_SQUARE_MARGIN/2, posY-TERRAIN_SQUARE_MARGIN/2);
	pixmapItem.setZValue(0);
}

TerrainType TerrainSquare::getTerrainType() const
{
	return terrainType;
}
