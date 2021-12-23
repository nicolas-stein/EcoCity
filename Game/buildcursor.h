#ifndef BUILDCURSOR_H
#define BUILDCURSOR_H

#include "Game/Grid/gridsquare.h"
#include "Game/Grid/roadsquare.h"
#include "Game/Grid/zonesquare.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QMap>
#include <QApplication>

class BuildCursor : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	BuildCursor();
	void setSquareToBuild(GridSquare *newSquareToBuild);
	void setPosition(int x, int y);

	void setRoadGrid(RoadSquare ***newRoadGrid);

	void setZoneGrid(ZoneSquare ***newZoneGrid);

private :
	GridSquare *squareToBuild = nullptr;

private:
	RoadSquare ***roadGrid;
	QMap<RoadType, QMap<RoadAngle, QPixmap>> *roadPixmaps;

	ZoneSquare ***zoneGrid;

	// QGraphicsItem interface
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
	void requestBuildSquare(GridSquare *square);
	void changeZoneSquareType(ZoneSquare *zoneSquare);
};

#endif // BUILDCURSOR_H
