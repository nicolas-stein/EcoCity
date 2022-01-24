#ifndef RESIDENTIALBUILDING_H
#define RESIDENTIALBUILDING_H

#include "zonebuilding.h"

//Classe représentant un batiment résidentiel
class ResidentialBuilding : public ZoneBuilding
{
public:
	ResidentialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager);

	int getResidents() const;
	int getMaxResidents() const;
	void changeResidents(int amountResidents);

	double getPollution() const;

private :
	int residents;
	int maxResidents;
};

#endif // RESIDENTIALBUILDING_H
