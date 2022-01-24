#include "powerbuilding.h"

PowerBuilding::PowerBuilding(int posX, int posY, RessourceManager *ressourceManager, Buildings::Service::PowerType powerType): ServiceBuilding(posX, posY, Buildings::Service::Type::Power, ressourceManager), powerType(powerType)
{
	//On défini la taille du batiment en fonction de son type (chaque type a une taille prédéfinie)
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

	//Les batiments électrique sont toujours connectés à l'électricité
	connectedToPower = true;

	currentPowerProduction = basePowerProduction;
}

Buildings::Service::PowerType PowerBuilding::getPowerType() const
{
	return powerType;
}

//Mise à jour de la texture du batiment
void PowerBuilding::updatePixmap(bool showToolTips)
{
	//Si le batiment n'est pas connecté à la route, on affiche une info-bulle avec un logo de route barré sur la texture de base du batiment
	if(!connectedToRoad && showToolTips){
		QPixmap pixmap = ressourceManager->getPowerBuildingPixmaps()->value(powerType);
		QPixmap tooltip = ressourceManager->getTooltipsPixmaps()->value(NoRoad);
		QPainter painter(&pixmap);
		painter.drawPixmap(pixmap.width()/2-tooltip.width()/2, pixmap.height()/2-tooltip.height()/2, tooltip);
		pixmapItem.setPixmap(pixmap);
	}
	else{	//Sinon on affiche la texture de base du batiment
		pixmapItem.setPixmap(ressourceManager->getPowerBuildingPixmaps()->value(powerType));
	}
}

double PowerBuilding::getPollution() const
{
	//Le batiment ne pollue pas s'il n'est pas en fonctionnement (connecté à la route)
	if(!connectedToRoad){
		return 0;
	}

	//Les batiments on des pollutions différentes en fonction de leur type
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
	//Si le batiment n'est pas connecté à la route il ne fonctionne pas => pas de production d'énergie
	if(!connectedToRoad){
		return 0;
	}
	else if(powerType == Buildings::Service::SmallSolar || powerType == Buildings::Service::BigSolar || powerType == Buildings::Service::Wind){
		//Si c'est une station solaire ou une éolienne, la production varie par rapport à la dernière fois
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
	//Les batiments ont un différent cout de construction en fonction de leur type
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
	//Les batiments ont un différent cout de maintenance en fonction de leur type
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
