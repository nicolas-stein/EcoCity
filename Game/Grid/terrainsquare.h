#ifndef TERRAINSQUARE_H
#define TERRAINSQUARE_H

#include "gridsquare.h"

//Classe représentant un carré de terrain
class TerrainSquare : public GridSquare
{
public:
	TerrainSquare(double noise, int posX, int posY, RessourceManager *ressourceManager);
	Grid::Terrain::Type getTerrainType() const;

private :
	Grid::Terrain::Type terrainType;
};

#endif // TERRAINSQUARE_H
