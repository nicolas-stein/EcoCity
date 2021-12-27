#ifndef RESIDENTIALBUILDING_H
#define RESIDENTIALBUILDING_H

#include "building.h"

class ResidentialBuilding : public Building
{
public:
	ResidentialBuilding(int posX, int posY, int width, int height, int residents, int maxResidents, QMap<ZoneType, QList<QPixmap>> *zonePixmaps);
private :
	int residents;
	int maxResidents;
};

#endif // RESIDENTIALBUILDING_H
