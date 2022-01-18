#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->tabWidget_main->setVisible(false);
	ui->statusBar->setVisible(false);

	mainMenu_graphicsScene = new MainMenuScene(this);
	connect(mainMenu_graphicsScene, &MainMenuScene::requestStart, this, &MainWindow::startGame);
	connect(mainMenu_graphicsScene, &MainMenuScene::requestQuit, this, &MainWindow::quitApp);

	ui->graphicsView->setScene(mainMenu_graphicsScene);
	setAttribute(Qt::WA_AlwaysShowToolTips, true);

	audioManager.startMusic();
	audioManager.start();
}

MainWindow::~MainWindow()
{
	audioManager.quit();
	delete ui;
	delete mainMenu_graphicsScene;
	if(mainGame_graphicsScene!=nullptr){
		delete mainGame_graphicsScene;
	}
}

void MainWindow::startGame()
{
	if(ui->graphicsView->scene() == mainMenu_graphicsScene && mainGame_graphicsScene == nullptr){
		ui->tabWidget_main->setVisible(true);
		ui->statusBar->setVisible(true);
		ui->tabWidget_main->setCurrentIndex(0);
		ui->tabWidget_construction->setCurrentIndex(0);
		mainGame_graphicsScene = new MainGameScene(ui->graphicsView);
		connect(mainGame_graphicsScene, SIGNAL(gameDateChanged(QDate)), this, SLOT(gameDateChanged(QDate)));
		connect(mainGame_graphicsScene, SIGNAL(gameDemandsUpdated(double,int,double,double)), this, SLOT(gameDemandsUpdated(double,int,double,double)));
		connect(mainGame_graphicsScene, SIGNAL(playSoundEffect(SoundEffects)), &audioManager, SLOT(playSoundEffect(SoundEffects)));

		ui->graphicsView->setScene(mainGame_graphicsScene);
		ui->graphicsView->setPanEnable(true);
		ui->graphicsView->setZoomEnbale(true);
		ui->graphicsView->setResizeEnable(true);
		ui->graphicsView->fitInView(mainGame_graphicsScene->width()/4, mainGame_graphicsScene->height()/4, mainGame_graphicsScene->width()/2, mainGame_graphicsScene->height()/2, Qt::KeepAspectRatio);
		mainGame_graphicsScene->startGameLogic();
	}
}


void MainWindow::quitApp(){
	qApp->quit();
}

void MainWindow::on_button_place_road_1_clicked(bool checked)
{
	if(checked){
		ui->button_place_road_2->setEnabled(false);
		ui->button_place_road_3->setEnabled(false);
		ui->button_remove_road->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		ui->tabWidget_construction->setTabEnabled(2, false);
		mainGame_graphicsScene->enableBuildMode(TwoLanes);
	}
	else{
		ui->button_place_road_2->setEnabled(true);
		ui->button_place_road_3->setEnabled(true);
		ui->button_remove_road->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		ui->tabWidget_construction->setTabEnabled(2, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}


void MainWindow::on_button_place_road_2_clicked(bool checked)
{
	if(checked){
		ui->button_place_road_1->setEnabled(false);
		ui->button_place_road_3->setEnabled(false);
		ui->button_remove_road->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		ui->tabWidget_construction->setTabEnabled(2, false);
		mainGame_graphicsScene->enableBuildMode(FourLanes);
	}
	else{
		ui->button_place_road_1->setEnabled(true);
		ui->button_place_road_3->setEnabled(true);
		ui->button_remove_road->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		ui->tabWidget_construction->setTabEnabled(2, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}


void MainWindow::on_button_place_road_3_clicked(bool checked)
{
	if(checked){
		ui->button_place_road_1->setEnabled(false);
		ui->button_place_road_2->setEnabled(false);
		ui->button_remove_road->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		ui->tabWidget_construction->setTabEnabled(2, false);
		mainGame_graphicsScene->enableBuildMode(Highway);
	}
	else{
		ui->button_place_road_1->setEnabled(true);
		ui->button_place_road_2->setEnabled(true);
		ui->button_remove_road->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		ui->tabWidget_construction->setTabEnabled(2, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}


void MainWindow::on_button_remove_road_clicked(bool checked)
{
	if(checked){
		ui->button_place_road_1->setEnabled(false);
		ui->button_place_road_2->setEnabled(false);
		ui->button_place_road_3->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		ui->tabWidget_construction->setTabEnabled(2, false);
		mainGame_graphicsScene->enableDestroyMode(GridRoad);
	}
	else{
		ui->button_place_road_1->setEnabled(true);
		ui->button_place_road_2->setEnabled(true);
		ui->button_place_road_3->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		ui->tabWidget_construction->setTabEnabled(2, true);
		mainGame_graphicsScene->disableDestroyMode();
	}
}

void MainWindow::on_button_place_zone_1_clicked(bool checked)
{
	if(checked){
		ui->button_place_zone_2->setEnabled(false);
		ui->button_place_zone_3->setEnabled(false);
		ui->button_remove_zone->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(2, false);
		mainGame_graphicsScene->enableBuildMode(Residential);
	}
	else{
		ui->button_place_zone_2->setEnabled(true);
		ui->button_place_zone_3->setEnabled(true);
		ui->button_remove_zone->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(2, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}


void MainWindow::on_button_place_zone_2_clicked(bool checked)
{
	if(checked){
		ui->button_place_zone_1->setEnabled(false);
		ui->button_place_zone_3->setEnabled(false);
		ui->button_remove_zone->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(2, false);
		mainGame_graphicsScene->enableBuildMode(Commercial);
	}
	else{
		ui->button_place_zone_1->setEnabled(true);
		ui->button_place_zone_3->setEnabled(true);
		ui->button_remove_zone->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(2, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}


void MainWindow::on_button_place_zone_3_clicked(bool checked)
{
	if(checked){
		ui->button_place_zone_1->setEnabled(false);
		ui->button_place_zone_2->setEnabled(false);
		ui->button_remove_zone->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(2, false);
		mainGame_graphicsScene->enableBuildMode(Industrial);
	}
	else{
		ui->button_place_zone_1->setEnabled(true);
		ui->button_place_zone_2->setEnabled(true);
		ui->button_remove_zone->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(2, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}


void MainWindow::on_button_remove_zone_clicked(bool checked)
{
	if(checked){
		ui->button_place_zone_1->setEnabled(false);
		ui->button_place_zone_2->setEnabled(false);
		ui->button_place_zone_3->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(2, false);
		mainGame_graphicsScene->enableBuildMode(None);
	}
	else{
		ui->button_place_zone_1->setEnabled(true);
		ui->button_place_zone_2->setEnabled(true);
		ui->button_place_zone_3->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(2, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}

void MainWindow::on_button_place_power_1_clicked(bool checked)
{
	if(checked){
		ui->button_place_power_2->setEnabled(false);
		ui->button_remove_power->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableBuildMode(SolarPower);
	}
	else{
		ui->button_place_power_2->setEnabled(true);
		ui->button_remove_power->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}

void MainWindow::on_button_place_power_2_clicked(bool checked)
{
	if(checked){
		ui->button_place_power_1->setEnabled(false);
		ui->button_remove_power->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableBuildMode(NuclearPower);
	}
	else{
		ui->button_place_power_1->setEnabled(true);
		ui->button_remove_power->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}


void MainWindow::on_button_remove_power_clicked(bool checked)
{
	if(checked){
		ui->button_place_power_1->setEnabled(false);
		ui->button_place_power_2->setEnabled(false);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableDestroyMode(GridBuilding);
	}
	else{
		ui->button_place_power_1->setEnabled(true);
		ui->button_place_power_2->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableDestroyMode();
	}
}

void MainWindow::on_tabWidget_construction_currentChanged(int index)
{
	if(mainGame_graphicsScene!=nullptr){
		mainGame_graphicsScene->showZones(index==1);
	}
}


void MainWindow::on_tabWidget_main_currentChanged(int index)
{
	if(index == 0){
		ui->statusBar->clearMessage();
		mainGame_graphicsScene->setGameSpeed(gameSpeed);
		if(ui->button_place_road_1->isChecked()){
			ui->button_place_road_1->setChecked(false);
			on_button_place_road_1_clicked(false);
		}
		if(ui->button_place_road_2->isChecked()){
			ui->button_place_road_2->setChecked(false);
			on_button_place_road_2_clicked(false);
		}
		if(ui->button_place_road_3->isChecked()){
			ui->button_place_road_3->setChecked(false);
			on_button_place_road_3_clicked(false);
		}
		if(ui->button_remove_road->isChecked()){
			ui->button_remove_road->setChecked(false);
			on_button_remove_road_clicked(false);
		}

		if(ui->button_place_zone_1->isChecked()){
			ui->button_place_zone_1->setChecked(false);
			on_button_place_zone_1_clicked(false);
		}
		if(ui->button_place_zone_2->isChecked()){
			ui->button_place_zone_2->setChecked(false);
			on_button_place_zone_2_clicked(false);
		}
		if(ui->button_place_zone_3->isChecked()){
			ui->button_place_zone_3->setChecked(false);
			on_button_place_zone_3_clicked(false);
		}
		if(ui->button_remove_zone->isChecked()){
			ui->button_remove_zone->setChecked(false);
			on_button_remove_zone_clicked(false);
		}
		if(mainGame_graphicsScene!=nullptr){
			mainGame_graphicsScene->showZones(false);
		}
	}
	else if(index == 1){
		ui->statusBar->showMessage("Mode construction : jeu en pause");
		ui->statusBar->setStyleSheet("font-weight: bold; color: red");
		mainGame_graphicsScene->setGameSpeed(0);
		if(mainGame_graphicsScene!=nullptr){
			mainGame_graphicsScene->showZones(ui->tabWidget_construction->currentIndex()==1);
		}
	}
	else if(index == 2){
		ui->statusBar->clearMessage();
		mainGame_graphicsScene->setGameSpeed(gameSpeed);
		if(ui->button_place_power_1->isChecked()){
			ui->button_place_power_1->setChecked(false);
			on_button_place_power_1_clicked(false);
		}
		if(ui->button_place_power_2->isChecked()){
			ui->button_place_power_2->setChecked(false);
			on_button_place_power_2_clicked(false);
		}
		if(ui->button_remove_power->isChecked()){
			ui->button_remove_power->setChecked(false);
			on_button_remove_power_clicked(false);
		}
		if(mainGame_graphicsScene!=nullptr){
			mainGame_graphicsScene->showZones(false);
		}
	}
}

void MainWindow::gameDateChanged(QDate newDate)
{
	ui->progressBar_date->setMaximum(newDate.daysInMonth());
	ui->progressBar_date->setValue(newDate.day());
	ui->progressBar_date->setFormat(newDate.toString("MMM yyyy"));
}

void MainWindow::gameDemandsUpdated(double residential, int residents, double commercial, double industrial)
{
	ui->progressBar_residential->setValue(residential);
	ui->progressBar_commercial->setValue(commercial);
	ui->progressBar_industrial->setValue(industrial);

	QPalette palette = ui->label_population_variation->palette();
	if(residents-this->residents > 0){ //Residents have increased
		ui->label_population_variation->setText("+"+QString::number(residents - this->residents));
		palette.setColor(ui->label_population_variation->foregroundRole(), Qt::green);

	}
	else if(residents-this->residents == 0){
		ui->label_population_variation->setText("");
	}
	else{ //Residents have decreased
		ui->label_population_variation->setText("-"+QString::number(this->residents - residents));
		palette.setColor(ui->label_population_variation->foregroundRole(), Qt::red);
	}

	ui->label_population_variation->setPalette(palette);
	this->residents = residents;
	ui->label_population->setText("Population : "+QString::number(residents));
}

void MainWindow::on_pushButton_gameSpeed_clicked()
{
	if(gameSpeed == 2){
		gameSpeed = 0.5;
		ui->pushButton_gameSpeed->setText("Vitesse : x0.5");
	}
	else if(gameSpeed == 0.5){
		gameSpeed = 1;
		ui->pushButton_gameSpeed->setText("Vitesse : x1");
	}
	else{
		gameSpeed = 2;
		ui->pushButton_gameSpeed->setText("Vitesse : x2");
	}

	mainGame_graphicsScene->setGameSpeed(gameSpeed);
}

