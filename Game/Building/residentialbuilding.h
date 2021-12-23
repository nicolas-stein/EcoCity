#ifndef RESIDENTIALBUILDING_H
#define RESIDENTIALBUILDING_H

#include "building.h"

class ResidentialBuilding : public Building
{
public:
	ResidentialBuilding(int posX, int posY, int width, int height, int maxResidents, QMap<ZoneType, QList<QPixmap>> *zonePixmaps);
private :
	int residents = 0;
	int maxResidents;
};

#endif // RESIDENTIALBUILDING_H
