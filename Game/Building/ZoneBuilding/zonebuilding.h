#ifndef ZONEBUILDING_H
#define ZONEBUILDING_H

#include "../building.h"

class ZoneSquare;

class ZoneBuilding : public Building
{
public:
	ZoneBuilding(int posX, int posY, int width, int height, Grid::Zone::Type zoneType, RessourceManager *ressourceManager);

	void addCoveringZone(ZoneSquare* zoneSquare);
	const QList<ZoneSquare *> &getCoveringZones() const;

	Grid::Zone::Type getZoneType() const;
	void updatePixmap(bool showToolTips);

	double getPowerConsumption() const;
	void setConnectedToPower(bool isConnectedToPower);

protected:
	int widthGrid();
	int heightGrid();
	unsigned randomPixmapNumber;
	double powerConsumption = 0;

private :
	QList<ZoneSquare*> coveringZones;
	Grid::Zone::Type zoneType;
};

#endif // ZONEBUILDING_H
