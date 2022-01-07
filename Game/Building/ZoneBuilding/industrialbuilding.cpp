#include "industrialbuilding.h"

IndustrialBuilding::IndustrialBuilding(int posX, int posY, int width, int height, RessourceManager *ressourceManager): ZoneBuilding(posX, posY, width, height, Industrial, ressourceManager)
{
	int widthGrid = width/ZONE_SQUARE_SIZE;
	int heightGrid = height/ZONE_SQUARE_SIZE;
	pixmapItem.setPixmap(ressourceManager->getZoneBuildingPixmap(Industrial, widthGrid, heightGrid));
	if(widthGrid == 4 && heightGrid == 4){
		maxWorkers = 20;
	}
	else if((widthGrid == 2 && heightGrid == 3) || (widthGrid == 3 && heightGrid == 2)){
		maxWorkers = 15;
	}
}

int IndustrialBuilding::getMaxWorkers() const
{
	return maxWorkers;
}
