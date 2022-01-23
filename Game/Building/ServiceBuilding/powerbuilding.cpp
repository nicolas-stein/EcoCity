#include "powerbuilding.h"

PowerBuilding::PowerBuilding(int posX, int posY, RessourceManager *ressourceManager, Buildings::Service::PowerType powerType): ServiceBuilding(posX, posY, Buildings::Service::Type::Power, ressourceManager), powerType(powerType)
{
	switch(powerType){
	case Buildings::Service::Coal:
		width = ZONE_SQUARE_SIZE*12;
		height = ZONE_SQUARE_SIZE*8;
		basePowerProduction = 15;
		break;
	case Buildings::Service::Gas:
		width = ZONE_SQUARE_SIZE*8;
		height = ZONE_SQUARE_SIZE*8;
		basePowerProduction = 40;
		break;
	case Buildings::Service::Oil:
		width = ZONE_SQUARE_SIZE*24;
		height = ZONE_SQUARE_SIZE*8;
		basePowerProduction = 60;
		break;
	case Buildings::Service::Wind:
		width = ZONE_SQUARE_SIZE*4;
		height = ZONE_SQUARE_SIZE*4;
		basePowerProduction = 10;
		break;
	case Buildings::Service::SmallSolar:
		width = ZONE_SQUARE_SIZE*2;
		height = ZONE_SQUARE_SIZE*4;
		basePowerProduction = 5;
		break;
	case Buildings::Service::BigSolar:
		width = ZONE_SQUARE_SIZE*8;
		height = ZONE_SQUARE_SIZE*8;
		basePowerProduction = 20;
		break;
	case Buildings::Service::Nuclear:
		width = ZONE_SQUARE_SIZE*8;
		height = ZONE_SQUARE_SIZE*8;
		basePowerProduction = 250;
		break;

	}

	connectedToPower = true;
}

Buildings::Service::PowerType PowerBuilding::getPowerType() const
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

double PowerBuilding::getPollution() const
{
	if(!connectedToRoad){
		return 0;
	}

	switch(powerType){
	case Buildings::Service::Coal:
		return 8200;
		break;
	case Buildings::Service::Gas:
		return 4900;
		break;
	case Buildings::Service::Oil:
		return 7200;
		break;
	case Buildings::Service::Wind:
		return 40;
		break;
	case Buildings::Service::SmallSolar:
		return 20;
		break;
	case Buildings::Service::BigSolar:
		return 50;
		break;
	case Buildings::Service::Nuclear:
		return 30;
		break;
	}
	return 0;
}

double PowerBuilding::getCurrentPowerProduction()
{
	if(!connectedToRoad){
		return 0;
	}
	else if(currentPowerProduction == 0){
		currentPowerProduction = basePowerProduction;
	}
	else if(powerType == Buildings::Service::SmallSolar || powerType == Buildings::Service::BigSolar || powerType == Buildings::Service::Wind){
		currentPowerProduction = currentPowerProduction + basePowerProduction * ((double)(QRandomGenerator::global()->generate() % 5) - 2) / 100;
		if(currentPowerProduction > basePowerProduction){
			currentPowerProduction = basePowerProduction;
		}
		else if(currentPowerProduction < basePowerProduction*0.75){
			currentPowerProduction = basePowerProduction*0.75;
		}
	}

	return currentPowerProduction;
}

double PowerBuilding::getCost() const
{
	switch(powerType){
	case Buildings::Service::Coal:
		return 100e3;
		break;
	case Buildings::Service::Gas:
		return 300e3;
		break;
	case Buildings::Service::Oil:
		return 500e3;
		break;
	case Buildings::Service::Wind:
		return 400e3;
		break;
	case Buildings::Service::SmallSolar:
		return 250e3;
		break;
	case Buildings::Service::BigSolar:
		return 750e3;
		break;
	case Buildings::Service::Nuclear:
		return 2.5e6;
		break;

	}

	return 0;
}

double PowerBuilding::getOperationalCost() const
{
	switch(powerType){
	case Buildings::Service::Coal:
		return 4e3;
		break;
	case Buildings::Service::Gas:
		return 12.5e3;
		break;
	case Buildings::Service::Oil:
		return 30e3;
		break;
	case Buildings::Service::Wind:
		return 5e3;
		break;
	case Buildings::Service::SmallSolar:
		return 2e3;
		break;
	case Buildings::Service::BigSolar:
		return 5e3;
		break;
	case Buildings::Service::Nuclear:
		return 200e3;
		break;
	}

	return 0;
}

double PowerBuilding::getBasePowerProduction() const
{
	return basePowerProduction;
}
