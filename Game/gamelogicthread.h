#ifndef GAMELOGICTHREAD_H
#define GAMELOGICTHREAD_H

#include "Constants.h"
#include "Grid/terrainsquare.h"
#include "Grid/roadsquare.h"
#include "Grid/zonesquare.h"
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
	void requestDestroy(GridSquare *gridSquare);

	void setGameSpeed(double newGameSpeed);

protected:
	void run();

private:

	void removeZoneSquare(int zoneX, int zoneY);

	QTimer *updateTimer;
	TerrainSquare ***terrainGrid;
	RoadSquare ***roadGrid;
	ZoneSquare ***zoneGrid;

	QMap<TerrainType, QPixmap> terrainPixmaps;
	QMap<RoadAngle, QPixmap> twoLanesPixmaps;
	QMap<RoadType, QMap<RoadAngle, QPixmap>> roadPixmaps;
	QMap<ZoneType, QPixmap> zonePixmaps;

	QMap<ZoneType, QList<QPixmap>> buildingPixmaps;
	QList<QPixmap> residentialPixmaps;

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
};

#endif // GAMELOGICTHREAD_H
