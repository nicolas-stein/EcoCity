#ifndef RESSOURCEMANAGER_H
#define RESSOURCEMANAGER_H

#include "constants.h"

#include <QMap>
#include <QPixmap>
#include <QRandomGenerator>
#include <QDebug>

class RessourceManager
{
public:
	RessourceManager();

	void loadRessources();

	QMap<TerrainType, QPixmap> *getTerrainPixmaps();

	QMap<RoadType, QMap<RoadAngle, QPixmap>> *getRoadPixmaps();

	QMap<ZoneType, QPixmap> *getZonePixmaps();

	QMap<ZoneType, QList<QPixmap>> *getZoneBuildingPixmaps();

	QPixmap getZoneBuildingPixmap(ZoneType zoneType, int widthGrid, int heightGrid);

	QMap<PowerType, QPixmap> *getPowerBuildingPixmaps();

private:
	QMap<TerrainType, QPixmap> terrainPixmaps;
	QMap<RoadAngle, QPixmap> twoLanesPixmaps;
	QMap<RoadType, QMap<RoadAngle, QPixmap>> roadPixmaps;
	QMap<ZoneType, QPixmap> zonePixmaps;

	QMap<ZoneType, QList<QPixmap>> zoneBuildingPixmaps;
	QList<QPixmap> residentialPixmaps;
	QList<QPixmap> commercialPixmaps;
	QList<QPixmap> industrialPixmaps;

	QMap<PowerType, QPixmap> powerBuildingPixmaps;

	void loadTerrainPixmaps();
	void loadZonePixmaps();

	void loadTwoLaneRoadPixmaps();

	void loadResidentialPixmaps();
	void loadCommercialPixmaps();
	void loadIndustrialPixmaps();

	void loadPowerPixmaps();
};

#endif // RESSOURCEMANAGER_H
