#include "powerbuilding.h"

PowerBuilding::PowerBuilding(int posX, int posY, RessourceManager *ressourceManager, PowerType powerType): ServiceBuilding(posX, posY, Power, ressourceManager), powerType(powerType)
{
	if(powerType == SolarPower){
		width = ZONE_SQUARE_SIZE*8;
		height = ZONE_SQUARE_SIZE*8;
		powerProduction = 20;
	}
	else if(powerType == NuclearPower){
		width = ZONE_SQUARE_SIZE*8;
		height = ZONE_SQUARE_SIZE*8;
		powerProduction = 100;
	}
	connectedToPower = true;
}

PowerType PowerBuilding::getPowerType() const
{
	return powerType;
}

void PowerBuilding::updatePixmap(bool showToolTips)
{
	if(!connectedToRoad && showToolTips){
		QPixmap pixmap = ressourceManager->getPowerBuildingPixmaps()->value(powerType);
		QPixmap tooltip = ressourceManager->getTooltipsPixmaps()->value(NoRoad);
		QPainter painter(&pixmap);
		painter.drawPixmap(pixmap.width()/2-tooltip.width()/2, pixmap.height()/2-tooltip.height()/2, tooltip);
		pixmapItem.setPixmap(pixmap);
	}
	else{
		pixmapItem.setPixmap(ressourceManager->getPowerBuildingPixmaps()->value(powerType));
	}
}

int PowerBuilding::getPowerProduction() const
{
	if(!connectedToRoad){
		return 0;
	}

	return powerProduction;
}

double PowerBuilding::getCost() const
{
	if(powerType == SolarPower){
		return 25e3;
	}
	else{
		return 100e3;
	}
}

double PowerBuilding::getOperationalCost() const
{
	if(powerType == SolarPower){
		return 5e3;
	}
	else{
		return 20e3;
	}
}
