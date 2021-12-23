#ifndef TERRAINSQUARE_H
#define TERRAINSQUARE_H

#include "gridsquare.h"

class TerrainSquare : public GridSquare
{
public:
	TerrainSquare(double noise, int posX, int posY, QMap<TerrainType, QPixmap> *terrainPixmaps);
	TerrainType getTerrainType() const;

private :
	TerrainType terrainType;
	QMap<TerrainType, QPixmap> *terrainPixmaps;
};

#endif // TERRAINSQUARE_H
