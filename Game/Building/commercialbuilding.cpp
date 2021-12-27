#include "commercialbuilding.h"

CommercialBuilding::CommercialBuilding(int posX, int posY, int width, int height, int workers, int maxWorkers, QMap<ZoneType, QList<QPixmap> > *zonePixmaps): Building(posX, posY, width, height, Commercial, zonePixmaps), workers(workers), maxWorkers(maxWorkers)
{
	int widthGrid = width/ZONE_SQUARE_SIZE;
	int heightGrid = height/ZONE_SQUARE_SIZE;
	if(widthGrid == 4 && heightGrid == 4){
		pixmapItem.setPixmap(zonePixmaps->value(Commercial).value(0));
	}
	else if(widthGrid == 1 && heightGrid == 2){
		pixmapItem.setPixmap(zonePixmaps->value(Commercial).value(1));
	}
	else if(widthGrid == 2 && heightGrid == 1){
		pixmapItem.setPixmap(zonePixmaps->value(Commercial).value(2));
	}
}
