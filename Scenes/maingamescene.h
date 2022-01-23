#ifndef MAINGAMESCENE_H
#define MAINGAMESCENE_H

#include "constants.h"
#include "Game/Building/building.h"
#include "customgraphicsview.h"
#include "Game/gamelogicthread.h"
#include "Game/buildcursor.h"
#include "Game/Grid/zonesquare.h"
#include "Game/Building/ServiceBuilding/powerbuilding.h"
#include "customgraphicspixmapitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>

class MainGameScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit MainGameScene(QObject *parent = nullptr);
	~MainGameScene();

	void startGameLogic();
	void enableBuildMode(Grid::Road::Type roadType);
	void enableBuildMode(Grid::Zone::Type zoneType);
	void enableBuildMode(Buildings::Service::PowerType powerType);
	void disableBuildMode();

	void enableDestroyMode(Grid::Type gridType);
	void disableDestroyMode();

	void showZones(bool enabled);

	void setGameSpeed(double newGameSpeed);

private:
	GameLogicThread gameLogicThread;

	CustomGraphicsPixmapItem ***terrainPixmapItems;
	QGraphicsItemGroup terrainPixmapItemGroup;

	CustomGraphicsPixmapItem ***roadPixmapItems = nullptr;
	CustomGraphicsPixmapItem ***zonePixmapItems = nullptr;

	QList<CustomGraphicsPixmapItem*> buildingList;

	BuildCursor buildCursor;

	bool buildingMode = false;
	bool destroyMode = false;
	Grid::Type destroyGridType;

	Building *buildingToBuild = nullptr;
	GridSquare *squareToBuild = nullptr;

private slots:
	void requestBuildSquare(GridSquare *square);
	void requestBuildBuilding(Building *building);
	void zoneSquareCreated(ZoneSquare *zoneSquare);
	void zoneSquareRemoved(ZoneSquare *zoneSquare);
	void changeZoneSquareType(ZoneSquare *zoneSquare, bool wholeArea);
	void buildingCreated(Building *building);
	void buildingRemoved(Building *building);

	void pixmapRequestedPosChange(qreal x, qreal y);
	void pixmapRequestChangePixmap(const QPixmap &pixmap);

	// QGraphicsScene interface
protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

signals:
	void gameDateChanged(QDate newDate);
	void gameMoneyUpdated(double money);
	void gameDemandsUpdated(double residential, int residents, double commercial, double industrial);
	void gamePowerUpdated(double powerProduction, double powerConsumption);
	void gamePollutionUpdated(int pollution, int maxPollution);
	void playSoundEffect(SoundEffects soundEffect);
	void changeStatusBarMessage(QString newMessage);
};

#endif // MAINGAMESCENE_H
