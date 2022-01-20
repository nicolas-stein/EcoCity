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
	~MapManager();

	void initMap(RessourceManager *ressourceManager);

	void removeZoneSquare(int zoneX, int zoneY);

	TerrainSquare ***getTerrainGrid();
	ZoneSquare ***getZoneGrid();
	RoadSquare ***getRoadGrid();
	int getRoadCount();
	QList<ZoneBuilding*> getZoneBuildings();
	QList<ServiceBuilding*> getServiceBuildings();
	Building *getBuildingFromPos(int posX, int posY);

	GridSquare *requestBuildSquare(GridSquare *square, double money);
	Building *requestBuildBuilding(Building *building, double money);
	void updateAdjacentRoadPixmaps(int x, int y, int recursive);
	void updateAdjacentRoadZones(RoadSquare *roadSquare);
	void requestDestroyRoad(RoadSquare *roadSquare);

	bool generateNewZoneBuilding(ZoneType zoneType);
	bool putZoneBuilding(int x, int y, int widthGrid, int heightGrid, ZoneType zoneType);
	bool canPutServiceBuilding(ServiceBuilding *building);
	bool isThereServiceBuilding(int x, int y);
	void requestDestroyBuilding(Building *building);

private:
	TerrainSquare ***terrainGrid;
	RoadSquare ***roadGrid;
	ZoneSquare ***zoneGrid;
	int roadCount = 0;

	QList<ZoneBuilding*> zoneBuildings;
	QList<ServiceBuilding*> serviceBuildings;

	RessourceManager *ressourceManager;

signals:
	void zoneSquareCreated(ZoneSquare *zoneSquare);
	void zoneSquareRemoved(ZoneSquare *zoneSquare);
	void buildingCreated(Building *newBuilding);
	void buildingRemoved(Building *building);
};

#endif // MAPMANAGER_H
