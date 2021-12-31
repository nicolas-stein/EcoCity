#ifndef INDUSTRIALBUILDING_H
#define INDUSTRIALBUILDING_H

#include "building.h"

#include <QRandomGenerator>

class IndustrialBuilding : public Building
{
public:
	IndustrialBuilding(int posX, int posY, int width, int height, int workers, int maxWorkers, QMap<ZoneType, QList<QPixmap>> *zonePixmaps);

private:
	int workers;
	int maxWorkers;
};

#endif // INDUSTRIALBUILDING_H
