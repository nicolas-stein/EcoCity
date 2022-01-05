#ifndef RESIDENTIALBUILDING_H
#define RESIDENTIALBUILDING_H

#include "building.h"

class ResidentialBuilding : public Building
{
public:
	ResidentialBuilding(int posX, int posY, int width, int height, QMap<ZoneType, QList<QPixmap>> *zonePixmaps);
	int getMaxResidents() const;

	int getResidents() const;

private :
	int residents;
	int maxResidents;
};

#endif // RESIDENTIALBUILDING_H
