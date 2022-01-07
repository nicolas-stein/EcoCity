#ifndef TERRAINSQUARE_H
#define TERRAINSQUARE_H

#include "gridsquare.h"

class TerrainSquare : public GridSquare
{
public:
	TerrainSquare(double noise, int posX, int posY, RessourceManager *ressourceManager);
	TerrainType getTerrainType() const;

private :
	TerrainType terrainType;
};

#endif // TERRAINSQUARE_H
