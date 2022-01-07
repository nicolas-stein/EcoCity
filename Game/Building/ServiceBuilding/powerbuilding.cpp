#include "powerbuilding.h"

PowerBuilding::PowerBuilding(int posX, int posY, RessourceManager *ressourceManager, PowerType powerType): ServiceBuilding(posX, posY, Power, ressourceManager), powerType(powerType)
{
	pixmapItem.setPixmap(ressourceManager->getPowerBuildingPixmaps()->value(powerType));
	if(powerType == SolarPower){
		width = ZONE_SQUARE_SIZE*8;
		height = ZONE_SQUARE_SIZE*8;
	}
	else if(powerType == NuclearPower){
		width = ZONE_SQUARE_SIZE*8;
		height = ZONE_SQUARE_SIZE*8;
	}
}

PowerType PowerBuilding::getPowerType() const
{
	return powerType;
}
