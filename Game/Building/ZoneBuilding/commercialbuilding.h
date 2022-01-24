#ifndef COMMERCIALBUILDING_H
#define COMMERCIALBUILDING_H

#include "zonebuilding.h"

//Classe représentant un batiment commercial
class CommercialBuilding : public ZoneBuilding
{
public:
	CommercialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager);

	int getMaxWorkers() const;
	int getMaxClients() const;
	double getPollution() const;

private:
	int maxWorkers;		//Nombre d'employés
	int maxClients;		//Nombre de clients que peut accueillir le batiment
};

#endif // COMMERCIALBUILDING_H
