#include "zonesquare.h"

ZoneSquare::ZoneSquare(int posX, int posY, ZoneType zoneType, RessourceManager *ressourceManager) : GridSquare(GridZone, posX, posY, ZONE_SQUARE_SIZE, ZONE_SQUARE_SIZE, ressourceManager), zoneType(zoneType)
{
	pixmapItem.setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	pixmapItem.setPixmap(ressourceManager->getZonePixmaps()->value(zoneType));
	pixmapItem.setPos(posX-ZONE_SQUARE_MARGIN/2, posY-ZONE_SQUARE_MARGIN/2);
	pixmapItem.setZValue(20);
	pixmapItem.setOpacity(0.6);
	pixmapItem.setVisible(false);
}

void ZoneSquare::setZoneType(ZoneType zoneType)
{
	this->zoneType = zoneType;
	pixmapItem.setPixmap(ressourceManager->getZonePixmaps()->value(zoneType));
}

ZoneType ZoneSquare::getZoneType() const
{
	return zoneType;
}

int ZoneSquare::isZoneAdjacentToRoad(RoadSquare ***roadGrid)
{
	/* Returns :
	 * Road on left : 4
	 * Road on right : 6
	 * Road on top : 8
	 * Road on bottom : 2
	 * No adjacent road : 0
	 */
	if(posX >= ROAD_SQUARE_SIZE && roadGrid[(posX-ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE][posY/ROAD_SQUARE_SIZE] != nullptr){
		return 4;
	}
	else if (posX < (ROAD_GRID_SIZE-1)*ROAD_SQUARE_SIZE && roadGrid[(posX+ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE][posY/ROAD_SQUARE_SIZE] != nullptr){
		return 6;
	}
	else if(posY >= ROAD_SQUARE_SIZE && roadGrid[posX/ROAD_SQUARE_SIZE][(posY-ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE] != nullptr){
		return 8;
	}
	else if(posY < (ROAD_GRID_SIZE-1)*ROAD_SQUARE_SIZE && roadGrid[posX/ROAD_SQUARE_SIZE][(posY+ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE] != nullptr){
		return 2;
	}
	return false;
}

bool ZoneSquare::isZoneValid(ZoneSquare ***zoneGrid, RoadSquare ***roadGrid)
{
	if(isZoneAdjacentToRoad(roadGrid)){
		return true;
	}
	if(posX > ROAD_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE-1][posY/ZONE_SQUARE_SIZE] != nullptr){
		if(roadGrid[(posX-2*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE][posY/ROAD_SQUARE_SIZE] != nullptr){
			return true;
		}
		else if(posX > ROAD_SQUARE_SIZE+ZONE_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE-2][posY/ZONE_SQUARE_SIZE] != nullptr){
			if(roadGrid[(posX-3*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE][posY/ROAD_SQUARE_SIZE] != nullptr){
				return true;
			}
			else if(posX > ROAD_SQUARE_SIZE+2*ZONE_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE-3][posY/ZONE_SQUARE_SIZE] != nullptr && roadGrid[(posX-4*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE][posY/ROAD_SQUARE_SIZE] != nullptr){
				return true;
			}
		}
	}
	if(posX < (ROAD_GRID_SIZE-1)*ROAD_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE+1][posY/ZONE_SQUARE_SIZE] != nullptr){
		if(roadGrid[(posX+2*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE][posY/ROAD_SQUARE_SIZE] != nullptr){
			return true;
		}
		else if(posX < (ROAD_GRID_SIZE-1)*ROAD_SQUARE_SIZE-ZONE_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE+2][posY/ZONE_SQUARE_SIZE] != nullptr){
			if(roadGrid[(posX+3*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE][posY/ROAD_SQUARE_SIZE] != nullptr){
				return true;
			}
			else if(posX < (ROAD_GRID_SIZE-1)*ROAD_SQUARE_SIZE-2*ZONE_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE+3][posY/ZONE_SQUARE_SIZE] != nullptr && roadGrid[(posX+4*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE][posY/ROAD_SQUARE_SIZE] != nullptr){
				return true;
			}
		}
	}
	if(posY > ROAD_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE][posY/ZONE_SQUARE_SIZE-1] != nullptr){
		if(roadGrid[posX/ROAD_SQUARE_SIZE][(posY-2*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE] != nullptr){
			return true;
		}
		else if(posY > ROAD_SQUARE_SIZE+ZONE_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE][posY/ZONE_SQUARE_SIZE-1] != nullptr){
			if(roadGrid[posX/ROAD_SQUARE_SIZE][(posY-3*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE] != nullptr){
				return true;
			}
			else if(posY > ROAD_SQUARE_SIZE+2*ZONE_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE][posY/ZONE_SQUARE_SIZE-1] != nullptr && roadGrid[posX/ROAD_SQUARE_SIZE][(posY-4*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE] != nullptr){
				return true;
			}
		}
	}
	if(posY < (ROAD_GRID_SIZE-1)*ROAD_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE][posY/ZONE_SQUARE_SIZE+1] != nullptr){
		if(roadGrid[posX/ROAD_SQUARE_SIZE][(posY+2*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE] != nullptr){
			return true;
		}
		else if(posY < (ROAD_GRID_SIZE-1)*ROAD_SQUARE_SIZE-ZONE_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE][posY/ZONE_SQUARE_SIZE+1] != nullptr){
			if(roadGrid[posX/ROAD_SQUARE_SIZE][(posY+3*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE] != nullptr){
				return true;
			}
			else if(posY < (ROAD_GRID_SIZE-1)*ROAD_SQUARE_SIZE-2*ZONE_SQUARE_SIZE && zoneGrid[posX/ZONE_SQUARE_SIZE][posY/ZONE_SQUARE_SIZE+1] != nullptr && roadGrid[posX/ROAD_SQUARE_SIZE][(posY+4*ZONE_SQUARE_SIZE)/ROAD_SQUARE_SIZE] != nullptr){
				return true;
			}
		}
	}
	return false;
}

ZoneBuilding *ZoneSquare::getBuilding()
{
	return building;
}

void ZoneSquare::setBuilding(ZoneBuilding *newBuilding)
{
	building = newBuilding;
}
