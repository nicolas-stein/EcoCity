#ifndef BUILDCURSOR_H
#define BUILDCURSOR_H

#include "constants.h"
#include "Game/ressourcemanager.h"
#include "Game/mapmanager.h"
#include "Game/Grid/gridsquare.h"
#include "Game/Building/building.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QApplication>


class BuildCursor : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	BuildCursor();
	~BuildCursor();
	void setSquareToBuild(GridSquare *newSquareToBuild);
	void setBuildingToBuild(Building *building);
	void setPosition(int x, int y);

	void setManagers(RessourceManager *ressourceManager, MapManager *mapManager);

private :
	GridSquare *squareToBuild = nullptr;
	Building *buildingToBuild = nullptr;

private:
	RessourceManager *ressourceManager;
	MapManager *mapManager;
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
	void requestBuildSquare(GridSquare *square);
	void requestBuildBuilding(Building *building);
	void changeZoneSquareType(ZoneSquare *zoneSquare);
};

#endif // BUILDCURSOR_H
