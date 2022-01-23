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

	QMap<Tooltips, QPixmap> *getTooltipsPixmaps();

	QMap<Grid::Terrain::Type, QPixmap> *getTerrainPixmaps();

	QMap<Grid::Road::Type, QMap<Grid::Road::Angle, QPixmap>> *getRoadPixmaps();

	QMap<Grid::Zone::Type, QPixmap> *getZonePixmaps();

	QMap<Grid::Zone::Type, QList<QPixmap>> *getZoneBuildingPixmaps();

	QPixmap getZoneBuildingPixmap(Grid::Zone::Type zoneType, int widthGrid, int heightGrid, unsigned randomPixmapNumber);

	QMap<Buildings::Service::PowerType, QPixmap> *getPowerBuildingPixmaps();

private:
	QMap<Tooltips, QPixmap> tooltipsPixmaps;

	QMap<Grid::Terrain::Type, QPixmap> terrainPixmaps;
	QMap<Grid::Road::Angle, QPixmap> twoLanesPixmaps;
	QMap<Grid::Road::Type, QMap<Grid::Road::Angle, QPixmap>> roadPixmaps;
	QMap<Grid::Zone::Type, QPixmap> zonePixmaps;

	QMap<Grid::Zone::Type, QList<QPixmap>> zoneBuildingPixmaps;
	QList<QPixmap> residentialPixmaps;
	QList<QPixmap> commercialPixmaps;
	QList<QPixmap> industrialPixmaps;

	QMap<Buildings::Service::PowerType, QPixmap> powerBuildingPixmaps;

	void loadTooltipsPixmaps();

	void loadTerrainPixmaps();
	void loadZonePixmaps();

	void loadTwoLaneRoadPixmaps();

	void loadResidentialPixmaps();
	void loadCommercialPixmaps();
	void loadIndustrialPixmaps();

	void loadPowerPixmaps();
};

#endif // RESSOURCEMANAGER_H
