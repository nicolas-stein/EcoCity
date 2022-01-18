#include "residentialbuilding.h"

ResidentialBuilding::ResidentialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager): ZoneBuilding(posX, posY, width, height, Residential, ressourceManager)
{
	residents = 0;
	if(widthGrid() == 4 && heightGrid() == 8){
		maxResidents = 100;
	}
	else if(widthGrid() == 4 && heightGrid() == 4){
		maxResidents = 8;
	}
	else if((widthGrid() == 1 && heightGrid() == 2) || (widthGrid() == 2 && heightGrid() == 1)){
		maxResidents = 3;
	}
}

int ResidentialBuilding::getMaxResidents() const
{
	return maxResidents;
}

int ResidentialBuilding::getResidents() const
{
	return residents;
}

void ResidentialBuilding::addResidents(int newResidents)
{
	if(residents + newResidents <= maxResidents){
		residents += newResidents;
	}
	powerConsumption = residents*11/1000;
}
