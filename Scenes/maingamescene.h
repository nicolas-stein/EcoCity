#ifndef MAINGAMESCENE_H
#define MAINGAMESCENE_H

#include "Game/gamelogicthread.h"
#include "customgraphicsview.h"
#include "Constants.h"
#include "Game/buildcursor.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QObject>
#include <QMap>
#include <QList>
#include <QGraphicsSceneMouseEvent>

class MainGameScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit MainGameScene(CustomGraphicsView *parent = nullptr);
	~MainGameScene();

	void startGameLogic();
	void enableBuildMode(RoadType roadType);
	void enableBuildMode(ZoneType zoneType);
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
	void requestBuildSquare(GridSquare *gridSquare);
	void zoneSquareCreated(ZoneSquare *zoneSquare);
	void zoneSquareRemoved(ZoneSquare *zoneSquare);
	void changeZoneSquareType(ZoneSquare *zoneSquare);
	void buildingCreated(Building *building);
	// QGraphicsScene interface
protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	// QGraphicsScene interface
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
	void gameDateChanged(QDate newDate);
};

#endif // MAINGAMESCENE_H
