#ifndef COMMERCIALBUILDING_H
#define COMMERCIALBUILDING_H

#include "zonebuilding.h"

class CommercialBuilding : public ZoneBuilding
{
public:
	CommercialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager);
	int getMaxWorkers() const;

	int getMaxClients() const;

private:
	int maxWorkers;
	int maxClients;
};

#endif // COMMERCIALBUILDING_H
