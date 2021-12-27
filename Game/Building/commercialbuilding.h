#ifndef COMMERCIALBUILDING_H
#define COMMERCIALBUILDING_H

#include "building.h"

class CommercialBuilding : public Building
{
public:
	CommercialBuilding(int posX, int posY, int width, int height, int workers, int maxWorkers, QMap<ZoneType, QList<QPixmap>> *zonePixmaps);
private:
	int workers;
	int maxWorkers;
};

#endif // COMMERCIALBUILDING_H
