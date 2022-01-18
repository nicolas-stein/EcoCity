#include "zonebuilding.h"

ZoneBuilding::ZoneBuilding(int posX, int posY, int width, int height, ZoneType zoneType, RessourceManager *ressourceManager): Building(posX, posY, width, height, BuildingZone, ressourceManager), zoneType(zoneType)
{
	randomPixmapNumber = QRandomGenerator::global()->generate();
}

void ZoneBuilding::addCoveringZone(ZoneSquare *zoneSquare)
{
	coveringZones.append(zoneSquare);
}

const QList<ZoneSquare *> &ZoneBuilding::getCoveringZones() const
{
	return coveringZones;
}

ZoneType ZoneBuilding::getZoneType() const
{
	return zoneType;
}

int ZoneBuilding::widthGrid()
{
	return width/ZONE_SQUARE_SIZE;

}

int ZoneBuilding::heightGrid()
{
	return height/ZONE_SQUARE_SIZE;
}

double ZoneBuilding::getPowerConsumption() const
{
	return powerConsumption;
}

void ZoneBuilding::updatePixmap(bool showToolTips)
{
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
	else{
		pixmapItem.setPixmap(ressourceManager->getZoneBuildingPixmap(zoneType, widthGrid(), heightGrid(), randomPixmapNumber));
	}
}

void ZoneBuilding::setConnectedToPower(bool isConnectedToPower)
{
	if(isConnectedToPower != connectedToPower){
		connectedToPower = isConnectedToPower;
		updatePixmap(true);
	}
	else{
		connectedToPower = isConnectedToPower;
	}
}

