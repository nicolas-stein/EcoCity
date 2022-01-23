#include <QDateTime>
#include "roadsquare.h"

RoadSquare::RoadSquare(int posX, int posY, Grid::Road::Type roadType, RessourceManager *ressourceManager): GridSquare(Grid::Type::GridRoad, posX, posY, ROAD_SQUARE_SIZE, ROAD_SQUARE_SIZE, ressourceManager), roadType(roadType)
{
	pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Cross));
	pixmapItem.setPos(posX-ROAD_SQUARE_MARGIN/2, posY-ROAD_SQUARE_MARGIN/2);
	pixmapItem.setZValue(1);
	pixmapItem.setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void RoadSquare::updatePixmap(RoadSquare ***roadGrid)
{
	int x = posX/ROAD_SQUARE_SIZE;
	int y = posY/ROAD_SQUARE_SIZE;

	connectedTop = y > 0 && roadGrid[x][y-1]!=nullptr;
	connectedBottom = y < ROAD_GRID_SIZE-1 &&roadGrid[x][y+1]!=nullptr;
	connectedLeft = x > 0 &&roadGrid[x-1][y] != nullptr;
	connectedRight = x < ROAD_GRID_SIZE-1 && roadGrid[x+1][y] != nullptr;

	if(connectedTop){
		if(connectedBottom){
			if(connectedLeft){
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Cross));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::TJunction).transformed(QTransform().rotate(90)));
				}
			}
			else{
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::TJunction).transformed(QTransform().rotate(-90)));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Straight).transformed(QTransform().rotate(90)));
				}
			}
		}
		else{
			if(connectedRight){
				if(connectedLeft){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::TJunction).transformed(QTransform().rotate(180)));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Turn).transformed(QTransform().rotate(-90)));
				}
			}
			else{
				if(connectedLeft){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Turn).transformed(QTransform().rotate(180)));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Straight).transformed(QTransform().rotate(-90)));	//Dead end from top
				}
			}
		}
	}
	else{
		if(connectedBottom){
			if(connectedLeft){
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::TJunction));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Turn).transformed(QTransform().rotate(90)));
				}
			}
			else{
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Turn));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Straight).transformed(QTransform().rotate(90))); //Dead end from bottom
				}
			}
		}
		else{
			if(connectedLeft){
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Straight));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Straight)); //Dead end from left
				}
			}
			else{
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Straight)); //Dead end from right
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Grid::Road::Angle::Cross)); //Nothing connected
				}
			}
		}
	}
}

Grid::Road::Type RoadSquare::getRoadType() const
{
	return roadType;
}

bool RoadSquare::isConnectedBottom() const
{
	return connectedBottom;
}

bool RoadSquare::isConnectedTop() const
{
	return connectedTop;
}

bool RoadSquare::isConnectedLeft() const
{
	return connectedLeft;
}

bool RoadSquare::isConnectedRight() const
{
	return connectedRight;
}

double RoadSquare::getCost() const
{
	return 1e3;
}
