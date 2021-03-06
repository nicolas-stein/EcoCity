#include "industrialbuilding.h"

IndustrialBuilding::IndustrialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager): ZoneBuilding(posX, posY, width, height, Grid::Zone::Type::Industrial, ressourceManager)
{
	if(widthGrid() == 4 && heightGrid() == 4){
		maxWorkers = 20;
		powerConsumption = 500e-3;
	}
	else if((widthGrid() == 2 && heightGrid() == 3) || (widthGrid() == 3 && heightGrid() == 2)){
		maxWorkers = 15;
		powerConsumption = 250e-3;
	}
}

int IndustrialBuilding::getMaxWorkers() const
{
	return maxWorkers;
}

double IndustrialBuilding::getPollution() const
{
	return maxWorkers * 3;
}
