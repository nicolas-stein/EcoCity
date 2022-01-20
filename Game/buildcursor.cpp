#include "buildcursor.h"

BuildCursor::BuildCursor()
{

}

BuildCursor::~BuildCursor()
{
	if(buildingToBuild!=nullptr){
		delete buildingToBuild;
	}
	if(squareToBuild != nullptr){
		delete squareToBuild;
	}
}

void BuildCursor::setSquareToBuild(GridSquare *newSquareToBuild)
{
	squareToBuild = newSquareToBuild;
	if(buildingToBuild != nullptr){
        delete buildingToBuild;
		buildingToBuild = nullptr;
	}
	setPixmap(squareToBuild->getPixmapItem()->pixmap());
	setZValue(100);
	if(squareToBuild->getGridType()==GridZone){
		setOpacity(0);
	}
	else{
		setOpacity(0.8);
	}
}

void BuildCursor::setBuildingToBuild(Building *building)
{
	if(squareToBuild != nullptr){
		delete squareToBuild;
		squareToBuild = nullptr;
	}
	buildingToBuild = building;
	buildingToBuild->updatePixmap(false);
	setPixmap(building->getPixmapItem()->pixmap());
	setZValue(100);
	setOpacity(0.8);
}

void BuildCursor::setPosition(int x, int y)
{
	if(squareToBuild != nullptr){
		if(squareToBuild->getGridType()==GridTerrain){
			x -= x % TERRAIN_SQUARE_SIZE;
			y -= y % TERRAIN_SQUARE_SIZE;
		}
		else if(squareToBuild->getGridType()==GridRoad){
			x -= x % ROAD_SQUARE_SIZE;
			y -= y % ROAD_SQUARE_SIZE;
		}
		else if(squareToBuild->getGridType()==GridZone){
			x -= x % ZONE_SQUARE_SIZE;
			y -= y % ZONE_SQUARE_SIZE;
		}

		if(x < 0 || y < 0 || x > scene()->width() || y > scene()->height()){
			return;
		}

		if(squareToBuild->getPosX()!=x || squareToBuild->getPosY()!=y){
			squareToBuild->setPos(x, y);
			setPos(squareToBuild->getPixmapItem()->x(), squareToBuild->getPixmapItem()->y());
			if(squareToBuild->getGridType()==GridRoad){
				((RoadSquare*)squareToBuild)->updatePixmap(mapManager->getRoadGrid());
				setPixmap(squareToBuild->getPixmapItem()->pixmap());
				if(QApplication::mouseButtons() == Qt::LeftButton){
					emit requestBuildSquare(squareToBuild);
				}
			}
			else if(squareToBuild->getGridType()==GridZone){
				if(mapManager->getZoneGrid()[x/ZONE_SQUARE_SIZE][y/ZONE_SQUARE_SIZE] != nullptr){
					setOpacity(1);
					if(QApplication::mouseButtons() == Qt::LeftButton){
						emit changeZoneSquareType((ZoneSquare*)squareToBuild, QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier));
					}
				}
				else{
					setOpacity(0);
				}
			}
		}
	}
	else if(buildingToBuild != nullptr){
		x -= buildingToBuild->getWidth()/2-ROAD_SQUARE_SIZE/2;
		x -= x % ROAD_SQUARE_SIZE;
		y -= buildingToBuild->getHeight()/2-ROAD_SQUARE_SIZE/2;
		y -= (y-buildingToBuild->getWidth()/2) % ROAD_SQUARE_SIZE;
		if(x < 0 || y < 0 || x > scene()->width()-buildingToBuild->getWidth() || y > scene()->height()-buildingToBuild->getHeight()){
			return;
		}

		if(buildingToBuild->getPosX()!=x || buildingToBuild->getPosY()!=y){
			buildingToBuild->setPos(x, y);
			setPos(buildingToBuild->getPixmapItem()->x(), buildingToBuild->getPixmapItem()->y());
		}
	}
}

void BuildCursor::setManagers(RessourceManager *ressourceManager, MapManager *mapManager)
{
	this->ressourceManager = ressourceManager;
	this->mapManager = mapManager;
}

void BuildCursor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button() == Qt::LeftButton){
		if(squareToBuild!=nullptr){
			if(squareToBuild->getGridType()==GridRoad){
				emit requestBuildSquare(squareToBuild);
				event->accept();
				return;
			}
			else if(squareToBuild->getGridType()==GridZone){
				emit changeZoneSquareType((ZoneSquare*)squareToBuild, QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier));
				event->accept();
				return;
			}
		}
		else if(buildingToBuild != nullptr){
			emit requestBuildBuilding(buildingToBuild);
			event->accept();
			return;
		}
	}
	event->ignore();
}
