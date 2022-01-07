#ifndef SERVICEBUILDING_H
#define SERVICEBUILDING_H

#include "constants.h"
#include "../building.h"

class ServiceBuilding : public Building
{
public:
	ServiceBuilding(int posX, int posY, ServiceBuildingType serviceType, RessourceManager *ressourceManager);

	ServiceBuildingType getServiceType() const;

private:
	ServiceBuildingType serviceType;
protected :
	int cost = 0;
};

#endif // SERVICEBUILDING_H
