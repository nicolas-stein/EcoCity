#include "commercialbuilding.h"

CommercialBuilding::CommercialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager): ZoneBuilding(posX, posY, width, height, Grid::Zone::Type::Commercial, ressourceManager)
{
	if(widthGrid() == 4 && heightGrid() == 4){
		maxWorkers = 10;
		maxClients = 30;
		powerConsumption = 300e-3;
	}
	else if((widthGrid() == 1 && heightGrid() == 2) || (widthGrid() == 2 && heightGrid() == 1)){
		maxWorkers = 3;
		maxClients = 5;
		powerConsumption = 80e-3;
	}
}

int CommercialBuilding::getMaxWorkers() const
{
	return maxWorkers;
}

int CommercialBuilding::getMaxClients() const
{
	return maxClients;
}

double CommercialBuilding::getPollution() const
{
	return maxWorkers * 3;
}
