#ifndef RESIDENTIALBUILDING_H
#define RESIDENTIALBUILDING_H

#include "zonebuilding.h"

class ResidentialBuilding : public ZoneBuilding
{
public:
	ResidentialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager);
	int getMaxResidents() const;

	int getResidents() const;
	void addResidents(int newResidents);

	double getPollution() const;

private :
	int residents;
	int maxResidents;
};

#endif // RESIDENTIALBUILDING_H
