#ifndef GAMELOGICTHREAD_H
#define GAMELOGICTHREAD_H

#include "Constants.h"
#include "Grid/terrainsquare.h"
#include "Grid/roadsquare.h"
#include "Grid/zonesquare.h"
#include "Game/Building/residentialbuilding.h"
#include "Game/Building/commercialbuilding.h"
#include "Game/Building/industrialbuilding.h"
#include "PerlinNoise.hpp"

#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QRandomGenerator>
#include <QMap>
#include <QPixmap>
#include <QDate>

class GameLogicThread : public QThread
{
	Q_OBJECT
public:
	GameLogicThread();

	// QThread interface
	TerrainSquare ***getTerrainGrid() const;

	RoadSquare ***getRoadGrid() const;

	QMap<RoadType, QMap<RoadAngle, QPixmap>>* getRoadPixmaps();
	QMap<ZoneType, QPixmap> *getZonePixmaps();

	ZoneSquare ***getZoneGrid() const;

	GridSquare *requestBuildSquare(GridSquare *square);
	void updateAdjacentRoadPixmaps(int x, int y, int recursive);
	void updateAdjacentRoadZones(RoadSquare *roadSquare);
	void requestDestroyRoad(RoadSquare *roadSquare);
	void requestDestroyBuilding(Building *building);

	void setGameSpeed(double newGameSpeed);
	void updateGameDemands();

protected:
	void run();

private:

	void removeZoneSquare(int zoneX, int zoneY);
	bool generateNewBuilding(ZoneType zoneType);
	bool putBuilding(int x, int y, int widthGrid, int heightGrid, ZoneType zoneType);

	QTimer *updateTimer;
	TerrainSquare ***terrainGrid;
	RoadSquare ***roadGrid;
	ZoneSquare ***zoneGrid;
	QList<Building*> buildingList;

	QMap<TerrainType, QPixmap> terrainPixmaps;
	QMap<RoadAngle, QPixmap> twoLanesPixmaps;
	QMap<RoadType, QMap<RoadAngle, QPixmap>> roadPixmaps;
	QMap<ZoneType, QPixmap> zonePixmaps;

	QMap<ZoneType, QList<QPixmap>> buildingPixmaps;
	QList<QPixmap> residentialPixmaps;
	QList<QPixmap> commercialPixmaps;
	QList<QPixmap> industrialPixmaps;

	int tickCounter = 0;
	QDate gameDate;
	double gameSpeed = 1;

private slots:
	void updateGameLogic();

signals:
	void zoneSquareCreated(ZoneSquare *zoneSquare);
	void zoneSquareRemoved(ZoneSquare *zoneSquare);
	void buildingCreated(Building *building);
	void gameDateChanged(QDate newDate);
	void gameDemandsUpdated(int res, int maxRes, int com, int maxCom, int indu, int maxIndu);
};

#endif // GAMELOGICTHREAD_H
