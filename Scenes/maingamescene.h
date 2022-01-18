#ifndef MAINGAMESCENE_H
#define MAINGAMESCENE_H

#include "constants.h"
#include "Game/Building/building.h"
#include "customgraphicsview.h"
#include "Game/gamelogicthread.h"
#include "Game/buildcursor.h"
#include "Game/Grid/zonesquare.h"
#include "Game/Building/ServiceBuilding/powerbuilding.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>

class MainGameScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit MainGameScene(CustomGraphicsView *parent = nullptr);
	~MainGameScene();

	void startGameLogic();
	void enableBuildMode(RoadType roadType);
	void enableBuildMode(ZoneType zoneType);
	void enableBuildMode(PowerType powerType);
	void disableBuildMode();

	void enableDestroyMode(GridType gridType);
	void disableDestroyMode();

	void showZones(bool enabled);

	void setGameSpeed(double newGameSpeed);

private:
	CustomGraphicsView *parent;
	GameLogicThread gameLogicThread;

	QGraphicsPixmapItem ***terrainPixmapItems;
	QGraphicsItemGroup terrainPixmapItemGroup;

	QGraphicsPixmapItem ***roadPixmapItems = nullptr;
	QGraphicsPixmapItem ***zonePixmapItems = nullptr;

	QList<QGraphicsPixmapItem*> buildingList;

	BuildCursor buildSquare;

	bool buildingMode = false;
	bool destroyMode = false;

private slots:
	void requestBuildSquare(GridSquare *square);
	void requestBuildBuilding(Building *building);
	void zoneSquareCreated(ZoneSquare *zoneSquare);
	void zoneSquareRemoved(ZoneSquare *zoneSquare);
	void changeZoneSquareType(ZoneSquare *zoneSquare);
	void buildingCreated(Building *building);
	void buildingRemoved(Building *building);
	// QGraphicsScene interface
protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	// QGraphicsScene interface
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
	void gameDateChanged(QDate newDate);
	void gameDemandsUpdated(double residential, int residents, double commercial, double industrial);
	void playSoundEffect(SoundEffects soundEffect);
};

#endif // MAINGAMESCENE_H
