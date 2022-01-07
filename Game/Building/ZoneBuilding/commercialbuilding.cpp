#include "commercialbuilding.h"

CommercialBuilding::CommercialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager): ZoneBuilding(posX, posY, width, height, Commercial, ressourceManager)
{
	int widthGrid = width/ZONE_SQUARE_SIZE;
	int heightGrid = height/ZONE_SQUARE_SIZE;

	pixmapItem.setPixmap(ressourceManager->getZoneBuildingPixmap(Commercial, widthGrid, heightGrid));

	if(widthGrid == 4 && heightGrid == 4){
		maxWorkers = 10;
		maxClients = 30;
	}
	else if((widthGrid == 1 && heightGrid == 2) || (widthGrid == 2 && heightGrid == 1)){
		maxWorkers = 3;
		maxClients = 5;
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
