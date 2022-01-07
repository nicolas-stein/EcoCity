#include "roadsquare.h"

RoadSquare::RoadSquare(int posX, int posY, RoadType roadType, RessourceManager *ressourceManager): GridSquare(GridRoad, posX, posY, ROAD_SQUARE_SIZE, ROAD_SQUARE_SIZE, ressourceManager), roadType(roadType)
{
	pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Cross));
	pixmapItem.setPos(posX-ROAD_SQUARE_MARGIN/2, posY-ROAD_SQUARE_MARGIN/2);
	pixmapItem.setZValue(10);
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
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Cross));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(TJunction).transformed(QTransform().rotate(90)));
				}
			}
			else{
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(TJunction).transformed(QTransform().rotate(-90)));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Straight).transformed(QTransform().rotate(90)));
				}
			}
		}
		else{
			if(connectedRight){
				if(connectedLeft){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(TJunction).transformed(QTransform().rotate(180)));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Turn).transformed(QTransform().rotate(-90)));
				}
			}
			else{
				if(connectedLeft){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Turn).transformed(QTransform().rotate(180)));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Straight).transformed(QTransform().rotate(-90)));	//Dead end from top
				}
			}
		}
	}
	else{
		if(connectedBottom){
			if(connectedLeft){
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(TJunction));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Turn).transformed(QTransform().rotate(90)));
				}
			}
			else{
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Turn));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Straight).transformed(QTransform().rotate(90))); //Dead end from bottom
				}
			}
		}
		else{
			if(connectedLeft){
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Straight));
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Straight)); //Dead end from left
				}
			}
			else{
				if(connectedRight){
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Straight)); //Dead end from right
				}
				else{
					pixmapItem.setPixmap(ressourceManager->getRoadPixmaps()->value(roadType).value(Cross)); //Nothing connected
				}
			}
		}
	}
}

RoadType RoadSquare::getRoadType() const
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
