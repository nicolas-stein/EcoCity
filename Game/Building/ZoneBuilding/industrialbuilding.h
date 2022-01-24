#ifndef INDUSTRIALBUILDING_H
#define INDUSTRIALBUILDING_H

#include "zonebuilding.h"

//Classe représentant un batiment industriel
class IndustrialBuilding : public ZoneBuilding
{
public:
	IndustrialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager);

	int getMaxWorkers() const;

	double getPollution() const;

private:
	int maxWorkers;
};

#endif // INDUSTRIALBUILDING_H
