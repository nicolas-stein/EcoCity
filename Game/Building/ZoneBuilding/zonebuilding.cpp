#include "zonebuilding.h"
#include <QThread>

ZoneBuilding::ZoneBuilding(int posX, int posY, int width, int height, Grid::Zone::Type zoneType, RessourceManager *ressourceManager): Building(posX, posY, width, height, Buildings::Type::BuildingZone, ressourceManager), zoneType(zoneType)
{
	randomPixmapNumber = QRandomGenerator::global()->generate();
}

Grid::Zone::Type ZoneBuilding::getZoneType() const
{
	return zoneType;
}


void ZoneBuilding::addCoveringZone(ZoneSquare *zoneSquare)
{
	coveringZones.append(zoneSquare);
}

const QList<ZoneSquare *> &ZoneBuilding::getCoveringZones() const
{
	return coveringZones;
}

//Mise à jour de la texture du batiment
void ZoneBuilding::updatePixmap(bool showToolTips)
{
	//Si le batiment n'est pas connecté à l'électricité , on affiche une info-bulle avec un logo d'éclaire barré sur la texture de base du batiment
	if(showToolTips && !connectedToPower){
		QPixmap pixmap = ressourceManager->getZoneBuildingPixmap(zoneType, widthGrid(), heightGrid(), randomPixmapNumber);
		QPixmap tooltip = ressourceManager->getTooltipsPixmaps()->value(NoPower);
		QPainter painter(&pixmap);
		if(pixmap.width() > pixmap.height()){
			painter.drawPixmap(pixmap.width()/2-pixmap.height()/2, pixmap.height()/2-pixmap.height()/2, pixmap.height(), pixmap.height(), tooltip);
		}
		else if(pixmap.width() < pixmap.height()){
			painter.drawPixmap(pixmap.width()/2-pixmap.width()/2, pixmap.height()/2-pixmap.width()/2, pixmap.width(), pixmap.width(),tooltip);
		}
		else{
			painter.drawPixmap(pixmap.width()/4, pixmap.height()/4, pixmap.width()/2, pixmap.height()/2, tooltip);
		}
		pixmapItem.setPixmap(pixmap);
	}
	else{	//Sinon on affiche la texture de base du batiment
		pixmapItem.setPixmap(ressourceManager->getZoneBuildingPixmap(zoneType, widthGrid(), heightGrid(), randomPixmapNumber));
	}
}

double ZoneBuilding::getPowerConsumption() const
{
	return powerConsumption;
}

void ZoneBuilding::setConnectedToPower(bool isConnectedToPower)
{
	//Si on a changé d'état, on met à jour la texture du batiment
	if(isConnectedToPower != connectedToPower){
		connectedToPower = isConnectedToPower;
		updatePixmap(true);
	}
}

int ZoneBuilding::widthGrid()
{
	return width/ZONE_SQUARE_SIZE;

}

int ZoneBuilding::heightGrid()
{
	return height/ZONE_SQUARE_SIZE;
}
