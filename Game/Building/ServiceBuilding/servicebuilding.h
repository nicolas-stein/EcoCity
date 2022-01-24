#ifndef SERVICEBUILDING_H
#define SERVICEBUILDING_H

#include "constants.h"
#include "../building.h"
#include "Game/Grid/roadsquare.h"

//Classe représentant un batiement "de service" (qui a une utilité fonctionnelle pour la ville)
class ServiceBuilding : public Building
{
public:
	ServiceBuilding(int posX, int posY, Buildings::Service::Type serviceType, RessourceManager *ressourceManager);

	Buildings::Service::Type getServiceType() const;

	void updateConnectedToRoad(RoadSquare ***roadGrid);

	bool isConnectedToRoad() const;

	virtual double getCost() const = 0;				//Cout de construction
	virtual double getOperationalCost() const = 0;	//Cout de maintenance (chaque mois)

private:
	Buildings::Service::Type serviceType;			//Type de batiment de service
protected :
	bool connectedToRoad = false;
};

#endif // SERVICEBUILDING_H
