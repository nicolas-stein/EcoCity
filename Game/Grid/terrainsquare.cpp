#include "terrainsquare.h"

TerrainSquare::TerrainSquare(double noise, int posX, int posY, QMap<TerrainType, QPixmap> *terrainPixmaps) : GridSquare(Terrain, posX, posY, TERRAIN_SQUARE_SIZE, TERRAIN_SQUARE_SIZE), terrainPixmaps(terrainPixmaps)
{
	if(noise < 0.25){
		terrainType = Water;
		pixmapItem.setPixmap(terrainPixmaps->value(Water));
	}
	else if(noise < 0.30){
		terrainType = Sand;
		pixmapItem.setPixmap(terrainPixmaps->value(Sand));
	}
	else if(noise < 0.77){
		terrainType = Grass;
		pixmapItem.setPixmap(terrainPixmaps->value(Grass));
	}
	else if(noise < 0.80){
		terrainType = Rock;
		pixmapItem.setPixmap(terrainPixmaps->value(Rock));
	}
	else{
		terrainType = Mountain;
		pixmapItem.setPixmap(terrainPixmaps->value(Mountain));
	}
	pixmapItem.setPos(posX-TERRAIN_SQUARE_MARGIN/2, posY-TERRAIN_SQUARE_MARGIN/2);
	pixmapItem.setZValue(0);
}

TerrainType TerrainSquare::getTerrainType() const
{
	return terrainType;
}
