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

//Classe représentant la scène principale du jeu
class MainGameScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit MainGameScene(QObject *parent = nullptr);
	~MainGameScene();

	void startGameLogic();											//On lance le thread qui gère la logique du jeu
	void enableBuildMode(Grid::Road::Type roadType);				//On active le mode construction pour une route
	void enableBuildMode(Grid::Zone::Type zoneType);				//On active le mode construction pour une zone
	void enableBuildMode(Buildings::Service::PowerType powerType);	//On active le mode construction pour un batiment d'électricité
	void disableBuildMode();										//On désactive le mode construction

	void enableDestroyMode(Grid::Type gridType);					//On active le mode destruction
	void disableDestroyMode();										//On désactive le mode destruction

	void showZones(bool enabled);									//On affiche (ou masque) les zones

	void setGameSpeed(double newGameSpeed);							//On change la vitesse du jeu

private:
	GameLogicThread gameLogicThread;						//Contient toute la logique du jeu

	CustomGraphicsPixmapItem ***terrainPixmapItems = nullptr;	//Tableau 2D de pointeurs vers des pixmapItem (qui affichent le terrain sur la scène)
	QGraphicsItemGroup terrainPixmapItemGroup;					//Groupe des terrainPixmapItems (pour optimiser les performances)

	CustomGraphicsPixmapItem ***roadPixmapItems = nullptr;		//Tableau 2D de pointeurs vers des pixmapItem (qui affichent les routes sur la scène)
	CustomGraphicsPixmapItem ***zonePixmapItems = nullptr;		//Tableau 2D de pointeurs vers des pixmapItem (qui affichent les zones sur la scène)

	QList<CustomGraphicsPixmapItem*> buildingList;				//Liste des pixmapItem des batiments (qui affichent les batiments sur la scène)

	BuildCursor buildCursor;				//Curseur de construction

	bool buildingMode = false;
	bool destroyMode = false;
	Grid::Type destroyGridType;				//Type de chose à détruire (route, batiment)

private slots:
	void requestBuildSquare(GridSquare *square);		//On a demandé à ce qu'un carré soit construit
	void requestBuildBuilding(Building *building);		//On a demandé à ce qu'un batiment soit construit
	void zoneSquareCreated(ZoneSquare *zoneSquare);		//Un carré de zone a été créé
	void zoneSquareRemoved(ZoneSquare *zoneSquare);		//Un carré de zone a été supprimé
	void changeZoneSquareType(ZoneSquare *zoneSquare, bool wholeArea);	//On a demandé à changer à un carré de zone de type
	void buildingCreated(Building *building);	//Un batiment a été créé par le gestionnaire de carte
	void buildingRemoved(Building *building);	//Un batiment a été supprimé par le gestionnaire de carte

	void pixmapRequestedPosChange(qreal x, qreal y);		//Un pixmapItem a besoin d'être changé de position
	void pixmapRequestChangePixmap(const QPixmap &pixmap);	//Un pixmapItem a besoin d'être changé de texture

	// QGraphicsScene interface
protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);	//Déplacement de la souris sur la scène
	void mousePressEvent(QGraphicsSceneMouseEvent *event);	//Clique de la souris sur la scène
	void resizeEvent(QResizeEvent *event);					//Redimensionnement de la scène

signals:
	void gameDateChanged(QDate newDate);	//On signal à la fênetre principal que la date du jeu a été changée
	void gameMoneyUpdated(double money);	//On signal à la fênetre principal que l'argent du joueur a été mis à jour
	void gameDemandsUpdated(double residential, int residents, double commercial, double industrial); //On signal à la fênetre principal que les demandes ont été mise à jour
	void gamePowerUpdated(double powerProduction, double powerConsumption); //On signal à la fênetre principal que l'électricité a été mise à jour
	void gamePollutionUpdated(int pollution);	//On signal à la fênetre principal que la pollution a été mise à jour
	void playSoundEffect(SoundEffects soundEffect);	//On signal à la fênetre principal que l'on veut jouer un effet sonore
	void changeStatusBarMessage(QString newMessage);//On signal à la fênetre principal que l'on veut changer le message de la barre de status
};

#endif // MAINGAMESCENE_H
