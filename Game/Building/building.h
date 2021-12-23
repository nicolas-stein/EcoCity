#ifndef BUILDING_H
#define BUILDING_H

#include "Constants.h"
#include <QGraphicsPixmapItem>
#include <QList>

class ZoneSquare;

class Building
{
public:
	Building(int posX, int posY, int width, int height, ZoneType buildingType, QMap<ZoneType, QList<QPixmap>> *zonePixmaps);

	void addCoveringZone(ZoneSquare* zoneSquare);

	QGraphicsPixmapItem *getPixmapItem();

	const QList<ZoneSquare *> &getCoveringZones() const;

private :
	int posX;
	int posY;
	int width;
	int height;
	ZoneType buildingType;
	QMap<ZoneType, QList<QPixmap>> *zonePixmaps;

	QList<ZoneSquare*> coveringZones;
protected :
	QGraphicsPixmapItem pixmapItem;
};

#endif // BUILDING_H
