#ifndef INDUSTRIALBUILDING_H
#define INDUSTRIALBUILDING_H

#include "building.h"

#include <QRandomGenerator>

class IndustrialBuilding : public Building
{
public:
	IndustrialBuilding(int posX, int posY, int width, int height, QMap<ZoneType, QList<QPixmap>> *zonePixmaps);

	int getMaxWorkers() const;

private:
	int maxWorkers;
};

#endif // INDUSTRIALBUILDING_H
