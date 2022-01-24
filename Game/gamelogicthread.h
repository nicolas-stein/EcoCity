#ifndef GAMELOGICTHREAD_H
#define GAMELOGICTHREAD_H

#include "constants.h"
#include "Game/ressourcemanager.h"
#include "Game/mapmanager.h"
#include "Game/Building/ZoneBuilding/residentialbuilding.h"
#include "Game/Building/ZoneBuilding/commercialbuilding.h"
#include "Game/Building/ZoneBuilding/industrialbuilding.h"

#include <QThread>
#include <QTimer>
#include <QDate>

//Classe représentant le thread gérant la partie "logique" du jeu
class GameLogicThread : public QThread
{
	Q_OBJECT
public:
	GameLogicThread(QObject *parent = nullptr);
	~GameLogicThread();

	RessourceManager *getRessourceManager();
	MapManager *getMapManager();

	void setGameSpeed(double newGameSpeed);
	void changeMoney(double amount);
	double getMoney() const;

protected:
	void run();

private:	
	QTimer *updateTimer;

	RessourceManager ressourceManager;	//Gestionnaire de ressources (contient toutes les textures du jeu)
	MapManager mapManager;				//Gestionnaire de la carte (s'occupe de toutes les cases et batiments du jeu)

	int tickCounter = 0;				//Nombre de tick (s'incrémente au fil du temps)
	QDate gameDate;						//Date dans le jeu
	double gameSpeed = 1;				//Vitesse du jeu
	double money = 150e3;				//Argent du joueur

	int totalResidents = 0;				//Nombre total de résident dans la ville
	int totalResidentialCapacity = 0;	//Capacité résidentielle maximale de la ville
	int totalWorkCapacity = 0;			//Nombre de postes dans la ville
	int totalCommercialCapacity = 0;	//Nombre de clients que peuvent accueillir les commerces

	int demandResidential= 40;			//Demande résidentielle
	int demandCommercial= 20;			//Demande commerciale
	int demandIndustrial= 20;			//Demande industrielle
	int demandEnergy = 0;				//Demande énergetique

	void updateGameDemands();			//Mise à jour des demandes
	void updateBuildingsPower(int *notPoweredBuildings, int *notConnectedToRoadBuildings);	//Mise à jour de l'électricité
	void updateGamePollution();			//Mise à jour de la pollution
	void changeResidents(int residents);	//Changement du nombre d'habitants
	void collectMoney(int daysInMonth);		//Mise à jour de l'argent

private slots:
	void updateGameLogic();				//Mise à jour de la partie logique du jeu

signals:
	void gameDateChanged(QDate newDate);	//Signal émis lorsque la date a été mise à jour
	void gameMoneyUpdated(double money);	//Signal émis lorsque l'argent a été mis à jour
	void gameDemandsUpdated(double residential, int residents, double commercial, double industrial);	//Signal émis lorsque les demandes ont été mise à jour
	void gamePowerUpdated(double powerProduction, double powerConsumption);	//Signal émis lorsque l'électricité a été mis à jour
	void gamePollutionUpdated(int pollution);	//Signal émis lorsque la pollution a été mise à jour
	void changeStatusBarMessage(QString newMessage);	//Signal émis lorsque l'on souhaite changer le message dans la barre d'état du jeu
};

#endif // GAMELOGICTHREAD_H
