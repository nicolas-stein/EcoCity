#include "residentialbuilding.h"

ResidentialBuilding::ResidentialBuilding(int posX, int posY, int width, int height, int residents, int maxResidents, QMap<ZoneType, QList<QPixmap>> *zonePixmaps): Building(posX, posY, width, height, Residential, zonePixmaps), residents(residents), maxResidents(maxResidents)
{
	int widthGrid = width/ZONE_SQUARE_SIZE;
	int heightGrid = height/ZONE_SQUARE_SIZE;
	if(widthGrid == 4 && heightGrid == 4){
		pixmapItem.setPixmap(zonePixmaps->value(Residential).value(0));
	}
	else if(widthGrid == 1 && heightGrid == 2){
		pixmapItem.setPixmap(zonePixmaps->value(Residential).value(1));
	}
	else if(widthGrid == 2 && heightGrid == 1){
		pixmapItem.setPixmap(zonePixmaps->value(Residential).value(2));
	}
}
