#ifndef SERVICEBUILDING_H
#define SERVICEBUILDING_H

#include "constants.h"
#include "../building.h"
#include "Game/Grid/roadsquare.h"

class ServiceBuilding : public Building
{
public:
	ServiceBuilding(int posX, int posY, ServiceBuildingType serviceType, RessourceManager *ressourceManager);

	ServiceBuildingType getServiceType() const;

	void updateConnectedToRoad(RoadSquare ***roadGrid);

	bool isConnectedToRoad() const;

	virtual double getCost() const = 0;
	virtual double getOperationalCost() const = 0;

private:
	ServiceBuildingType serviceType;
protected :
	int cost = 0;
	bool connectedToRoad = false;
};

#endif // SERVICEBUILDING_H
