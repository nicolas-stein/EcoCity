#ifndef COMMERCIALBUILDING_H
#define COMMERCIALBUILDING_H

#include "building.h"

class CommercialBuilding : public Building
{
public:
	CommercialBuilding(int posX, int posY, int width, int height, QMap<ZoneType, QList<QPixmap>> *zonePixmaps);
	int getMaxWorkers() const;

	int getMaxClients() const;

private:
	int maxWorkers;
	int maxClients;
};

#endif // COMMERCIALBUILDING_H
