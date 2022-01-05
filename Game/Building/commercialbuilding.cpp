#include "commercialbuilding.h"

CommercialBuilding::CommercialBuilding(int posX, int posY, int width, int height, QMap<ZoneType, QList<QPixmap> > *zonePixmaps): Building(posX, posY, width, height, Commercial, zonePixmaps)
{
	int widthGrid = width/ZONE_SQUARE_SIZE;
	int heightGrid = height/ZONE_SQUARE_SIZE;
	if(widthGrid == 4 && heightGrid == 4){
		pixmapItem.setPixmap(zonePixmaps->value(Commercial).value(0));
		maxWorkers = 10;
		maxClients = 30;
	}
	else if(widthGrid == 1 && heightGrid == 2){
		pixmapItem.setPixmap(zonePixmaps->value(Commercial).value(1));
		maxWorkers = 3;
		maxClients = 5;
	}
	else if(widthGrid == 2 && heightGrid == 1){
		pixmapItem.setPixmap(zonePixmaps->value(Commercial).value(2));
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
