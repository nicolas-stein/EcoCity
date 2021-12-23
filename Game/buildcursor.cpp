#include "buildcursor.h"

BuildCursor::BuildCursor()
{

}

void BuildCursor::setSquareToBuild(GridSquare *newSquareToBuild)
{
	squareToBuild = newSquareToBuild;
	setPixmap(squareToBuild->getPixmapItem()->pixmap());
	setZValue(100);
	if(squareToBuild->getGridType()==Zone){
		setOpacity(0);
	}
	else{
		setOpacity(0.8);
	}
}

void BuildCursor::setPosition(int x, int y)
{
	if(x < 0 || y < 0 || x > scene()->width() || y > scene()->height()){
		return;
	}
	if(squareToBuild->getGridType()==Terrain){
		x -= x % TERRAIN_SQUARE_SIZE;
		y -= y % TERRAIN_SQUARE_SIZE;
	}
	else if(squareToBuild->getGridType()==Road){
		x -= x % ROAD_SQUARE_SIZE;
		y -= y % ROAD_SQUARE_SIZE;
	}
	else if(squareToBuild->getGridType()==Zone){
		x -= x % ZONE_SQUARE_SIZE;
		y -= y % ZONE_SQUARE_SIZE;
	}

	if(squareToBuild->getPosX()!=x || squareToBuild->getPosY()!=y){
		squareToBuild->setPos(x, y);
		setPos(squareToBuild->getPixmapItem()->x(), squareToBuild->getPixmapItem()->y());
		if(squareToBuild->getGridType()==Road){
			((RoadSquare*)squareToBuild)->updatePixmap(roadGrid);
			setPixmap(squareToBuild->getPixmapItem()->pixmap());
			if(QApplication::mouseButtons() == Qt::LeftButton){
				emit requestBuildSquare(squareToBuild);
			}
		}
		else if(squareToBuild->getGridType()==Zone){
			if(zoneGrid[x/ZONE_SQUARE_SIZE][y/ZONE_SQUARE_SIZE] != nullptr){
				setOpacity(1);
				if(QApplication::mouseButtons() == Qt::LeftButton){
					emit changeZoneSquareType((ZoneSquare*)squareToBuild);
				}
			}
			else{
				setOpacity(0);
			}
		}
	}
}

void BuildCursor::setRoadGrid(RoadSquare ***newRoadGrid)
{
	roadGrid = newRoadGrid;
}

void BuildCursor::setZoneGrid(ZoneSquare ***newZoneGrid)
{
	zoneGrid = newZoneGrid;
}

void BuildCursor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button() == Qt::LeftButton){
		if(squareToBuild->getGridType()==Road){
			emit requestBuildSquare(squareToBuild);
			event->accept();
			return;
		}
		else if(squareToBuild->getGridType()==Zone){
			emit changeZoneSquareType((ZoneSquare*)squareToBuild);
			event->accept();
			return;
		}
	}
	event->ignore();
}
