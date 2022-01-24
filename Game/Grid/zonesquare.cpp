#include "zonesquare.h"

ZoneSquare::ZoneSquare(int posX, int posY, Grid::Zone::Type zoneType, RessourceManager *ressourceManager) : GridSquare(Grid::Type::GridZone, posX, posY, ZONE_SQUARE_SIZE, ZONE_SQUARE_SIZE, ressourceManager), zoneType(zoneType)
{
	pixmapItem.setCacheMode(QGraphicsItem::DeviceCoordinateCache);				//Cache de la texture afin d'améliorer les performances
	pixmapItem.setPixmap(ressourceManager->getZonePixmaps()->value(zoneType));	//On défini la texture utilisée par la zone
	pixmapItem.setPos(posX-ZONE_SQUARE_MARGIN/2, posY-ZONE_SQUARE_MARGIN/2);	//On défini la position du pixmapItem qui va être affiché sur la scène
	pixmapItem.setZValue(20);													//Elevation du pixmap (1er plan, second plan, troisième plan, ...)
	pixmapItem.setOpacity(0.6);													//Opacité du pixmap (0 = transparent, 1 = opaque)
	pixmapItem.setVisible(false);												//On rend invisble le pixmap pour ne pas l'afficher sur la carte par défaut
}

void ZoneSquare::setZoneType(Grid::Zone::Type zoneType)
{
	//Quand on change le type de la zone, on met aussi à jour le pixmap
	this->zoneType = zoneType;
	pixmapItem.setPixmap(ressourceManager->getZonePixmaps()->value(zoneType));
}

Grid::Zone::Type ZoneSquare::getZoneType() const
{
	return zoneType;
}

int ZoneSquare::isZoneAdjacentToRoad(RoadSquare ***roadGrid)
{
	/* Retourne :
	 * Route à gauche : 4
	 * Route à droite : 6
	 * Route en haut : 8
	 * Route en bas : 2
	 * Pas de route adjactente : 0
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
	/* Vérifie sur la zone est valide :
	 * Il faut que la zone soit collée à une route ou
	 * qu'il y ai dans une direction, au plus trois autres zone du même type
	 * avec la dernière collée à une route
	 */

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
