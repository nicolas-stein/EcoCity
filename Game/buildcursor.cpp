#include "buildcursor.h"

BuildCursor::BuildCursor(QObject *parent): QObject{parent}
{

}

BuildCursor::~BuildCursor()
{
	freeSquareBuildingToBuild();
}

void BuildCursor::freeSquareBuildingToBuild()
{
	//Si les variables sont définies, on les libères de la mémoire
	if(squareToBuild != nullptr){
		delete squareToBuild;
		squareToBuild = nullptr;
	}

	if(buildingToBuild != nullptr){
		delete buildingToBuild;
		buildingToBuild = nullptr;
	}
}

void BuildCursor::setSquareToBuild(GridSquare *newSquareToBuild)
{
	//On défini le carré qu'on veut construire
	freeSquareBuildingToBuild();
	squareToBuild = newSquareToBuild;
	squareToBuild->getPixmapItem()->mainThreadConnected();
	setPixmap(squareToBuild->getPixmapItem()->getLastPixmap());
	setZValue(100);
	if(squareToBuild->getGridType()==Grid::Type::GridZone){
		setOpacity(0);
	}
	else{
		setOpacity(0.8);
	}
}

void BuildCursor::setBuildingToBuild(Building *building)
{
	//On défini le batiment qu'on veut construire
	freeSquareBuildingToBuild();
	buildingToBuild = building;
	buildingToBuild->updatePixmap(false);
	setPixmap(building->getPixmapItem()->getLastPixmap());
	setZValue(100);
	setOpacity(0.8);
}

void BuildCursor::setPosition(int x, int y)
{
	//Lorsque l'on déplace la souris sur la scène, le curseur suit la souris
	//On met à jour les coordonnées du carré à construire (ou du batiment à construire)
	if(squareToBuild != nullptr){
		if(squareToBuild->getGridType()==Grid::Type::GridTerrain){
			x -= x % TERRAIN_SQUARE_SIZE;
			y -= y % TERRAIN_SQUARE_SIZE;
		}
		else if(squareToBuild->getGridType()==Grid::Type::GridRoad){
			x -= x % ROAD_SQUARE_SIZE;
			y -= y % ROAD_SQUARE_SIZE;
		}
		else if(squareToBuild->getGridType()==Grid::Type::GridZone){
			x -= x % ZONE_SQUARE_SIZE;
			y -= y % ZONE_SQUARE_SIZE;
		}

		if(x < 0 || y < 0 || x > scene()->width() || y > scene()->height()){
			return;
		}

		if(squareToBuild->getPosX()!=x || squareToBuild->getPosY()!=y){
			squareToBuild->setPos(x, y);
			squareToBuild->getPixmapItem()->mainThreadConnected();
			setPos(squareToBuild->getPixmapItem()->x(), squareToBuild->getPixmapItem()->y());
			if(squareToBuild->getGridType()==Grid::Type::GridRoad){
				((RoadSquare*)squareToBuild)->updatePixmap(mapManager->getRoadGrid());
				squareToBuild->getPixmapItem()->mainThreadConnected();
				setPixmap(squareToBuild->getPixmapItem()->pixmap());
				//Si on a le clique gauche d'enfoncé quand on a déplacé la souris, on demande la construction du carré
				if(QApplication::mouseButtons() == Qt::LeftButton){
					emit requestBuildSquare(squareToBuild);
				}
			}
			else if(squareToBuild->getGridType()==Grid::Type::GridZone){
				if(mapManager->getZoneGrid()[x/ZONE_SQUARE_SIZE][y/ZONE_SQUARE_SIZE] != nullptr){
					setOpacity(1);
					//Si on a le clique gauche d'enfoncé quand on a déplacé la souris, on demande le changement de type de zone
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
		y -=  y % ROAD_SQUARE_SIZE;
		if(x < 0 || y < 0 || x > scene()->width()-buildingToBuild->getWidth() || y > scene()->height()-buildingToBuild->getHeight()){
			return;
		}

		if(buildingToBuild->getPosX()!=x || buildingToBuild->getPosY()!=y){
			buildingToBuild->setPos(x, y);
			buildingToBuild->getPixmapItem()->mainThreadConnected();
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
	//Lorsque l'on clique avec le bouton gauche de la souris
	if(event->button() == Qt::LeftButton){
		if(squareToBuild!=nullptr){
			if(squareToBuild->getGridType()==Grid::Type::GridRoad){
				//On demande la construction du carré
				emit requestBuildSquare(squareToBuild);
				event->accept();
				return;
			}
			else if(squareToBuild->getGridType()==Grid::Type::GridZone){
				//On demande le changement de type de zone
				emit changeZoneSquareType((ZoneSquare*)squareToBuild, QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier));
				event->accept();
				return;
			}
		}
		else if(buildingToBuild != nullptr){
			//On demande la construction du batiment
			emit requestBuildBuilding(buildingToBuild);
			event->accept();
			return;
		}
	}
	event->ignore();
}
