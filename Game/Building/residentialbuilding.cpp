#include "residentialbuilding.h"

ResidentialBuilding::ResidentialBuilding(int posX, int posY, int width, int height, QMap<ZoneType, QList<QPixmap>> *zonePixmaps): Building(posX, posY, width, height, Residential, zonePixmaps)
{
	int widthGrid = width/ZONE_SQUARE_SIZE;
	int heightGrid = height/ZONE_SQUARE_SIZE;
	residents = 0;
	if(widthGrid == 4 && heightGrid == 8){
		pixmapItem.setPixmap(zonePixmaps->value(Residential).value(3));
		maxResidents = 32;
	}
	else if(widthGrid == 4 && heightGrid == 4){
		pixmapItem.setPixmap(zonePixmaps->value(Residential).value(0));
		maxResidents = 7;
	}
	else if(widthGrid == 1 && heightGrid == 2){
		pixmapItem.setPixmap(zonePixmaps->value(Residential).value(1));
		maxResidents = 3;
	}
	else if(widthGrid == 2 && heightGrid == 1){
		pixmapItem.setPixmap(zonePixmaps->value(Residential).value(2));
		maxResidents = 3;
	}
}

int ResidentialBuilding::getMaxResidents() const
{
	return maxResidents;
}

int ResidentialBuilding::getResidents() const
{
	return residents;
}
