#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "constants.h"
#include "PerlinNoise.hpp"
#include "Game/Grid/terrainsquare.h"
#include "Game/Grid/zonesquare.h"
#include "Game/Grid/roadsquare.h"
#include "Game/Building/ZoneBuilding/residentialbuilding.h"
#include "Game/Building/ZoneBuilding/commercialbuilding.h"
#include "Game/Building/ZoneBuilding/industrialbuilding.h"
#include "Game/Building/ServiceBuilding/powerbuilding.h"

#include <QObject>
#include <QList>

class MapManager: public QObject
{
	Q_OBJECT
public:
	MapManager();

	void initMap(RessourceManager *ressourceManager);

	void removeZoneSquare(int zoneX, int zoneY);

	TerrainSquare ***getTerrainGrid();
	ZoneSquare ***getZoneGrid();
	RoadSquare ***getRoadGrid();
	QList<ZoneBuilding*> getZoneBuildings();

	GridSquare *requestBuildSquare(GridSquare *square);
	Building *requestBuildBuilding(Building *building);
	void updateAdjacentRoadPixmaps(int x, int y, int recursive);
	void updateAdjacentRoadZones(RoadSquare *roadSquare);
	void requestDestroyRoad(RoadSquare *roadSquare);

	bool generateNewZoneBuilding(ZoneType zoneType);
	bool putZoneBuilding(int x, int y, int widthGrid, int heightGrid, ZoneType zoneType);
	void requestDestroyBuilding(Building *building);

private:
	TerrainSquare ***terrainGrid;
	RoadSquare ***roadGrid;
	ZoneSquare ***zoneGrid;

	QList<ZoneBuilding*> zoneBuildings;
	QList<ServiceBuilding*> serviceBuildings;

	RessourceManager *ressourceManager;

signals:
	void zoneSquareCreated(ZoneSquare *zoneSquare);
	void zoneSquareRemoved(ZoneSquare *zoneSquare);
	void buildingCreated(Building *newBuilding);
};

#endif // MAPMANAGER_H
