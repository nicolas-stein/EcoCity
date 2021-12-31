#include "industrialbuilding.h"

IndustrialBuilding::IndustrialBuilding(int posX, int posY, int width, int height, int workers, int maxWorkers, QMap<ZoneType, QList<QPixmap> > *zonePixmaps): Building(posX, posY, width, height, Industrial, zonePixmaps), workers(workers), maxWorkers(maxWorkers)
{
	int widthGrid = width/ZONE_SQUARE_SIZE;
	int heightGrid = height/ZONE_SQUARE_SIZE;
	if(widthGrid == 4 && heightGrid == 4){
		pixmapItem.setPixmap(zonePixmaps->value(Industrial).value(0+QRandomGenerator::global()->generate()%2));
	}
	else if(widthGrid == 2 && heightGrid == 3){
		pixmapItem.setPixmap(zonePixmaps->value(Industrial).value(2));
	}
	else if(widthGrid == 3 && heightGrid == 2){
		pixmapItem.setPixmap(zonePixmaps->value(Industrial).value(3));
	}
}
