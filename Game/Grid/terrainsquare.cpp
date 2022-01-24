#include "terrainsquare.h"

TerrainSquare::TerrainSquare(double noise, int posX, int posY, RessourceManager *ressourceManager) : GridSquare(Grid::Type::GridTerrain, posX, posY, TERRAIN_SQUARE_SIZE, TERRAIN_SQUARE_SIZE, ressourceManager)
{
	//Quand on créer un terrain, on passe un paramètre "noise" qui détermine l'élévation géographique du terrain
	//Une élévation proche de 0 signifie que c'est de l'eau, une élévation moyenne signifie que c'est de l'herbe et une élévation proche de 1 signifique c'est une montagne
	//On change la texture du terrain et son type en fonction de son élévation
	if(noise < 0.25){
		terrainType = Grid::Terrain::Type::Water;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Grid::Terrain::Type::Water));
	}
	else if(noise < 0.30){
		terrainType = Grid::Terrain::Type::Sand;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Grid::Terrain::Type::Sand));
	}
	else if(noise < 0.77){
		terrainType = Grid::Terrain::Type::Grass;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Grid::Terrain::Type::Grass));
	}
	else if(noise < 0.80){
		terrainType = Grid::Terrain::Type::Rock;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Grid::Terrain::Type::Rock));
	}
	else{
		terrainType = Grid::Terrain::Type::Mountain;
		pixmapItem.setPixmap(ressourceManager->getTerrainPixmaps()->value(Grid::Terrain::Type::Mountain));
	}
	pixmapItem.setPos(posX-TERRAIN_SQUARE_MARGIN/2, posY-TERRAIN_SQUARE_MARGIN/2);
	pixmapItem.setZValue(0);
}

Grid::Terrain::Type TerrainSquare::getTerrainType() const
{
	return terrainType;
}
