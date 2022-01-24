#include "residentialbuilding.h"

ResidentialBuilding::ResidentialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager): ZoneBuilding(posX, posY, width, height, Grid::Zone::Type::Residential, ressourceManager)
{
	residents = 0;
	if((widthGrid() == 4 && heightGrid() == 8) ||
			(widthGrid() == 8 && heightGrid() == 4)){
		maxResidents = 70;
	}
	else if(widthGrid() == 4 && heightGrid() == 4){
		maxResidents = 6;
	}
	else if((widthGrid() == 1 && heightGrid() == 2) || (widthGrid() == 2 && heightGrid() == 1)){
		maxResidents = 3;
	}
}

int ResidentialBuilding::getResidents() const
{
	return residents;
}

int ResidentialBuilding::getMaxResidents() const
{
	return maxResidents;
}

void ResidentialBuilding::changeResidents(int amountResidents)
{
	if((amountResidents > 0 && residents + amountResidents <= maxResidents) || (amountResidents < 0 && residents >= amountResidents)){
		residents += amountResidents;
	}

	powerConsumption = (double)residents*20/1000;
}

double ResidentialBuilding::getPollution() const
{
	return residents *2;
}
