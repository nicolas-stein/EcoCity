#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->tabWidget_main->setVisible(false);
	ui->statusBar->setVisible(false);
	ui->statusBar->addPermanentWidget(&statusBarLabel, 1);
	statusBarLabel.setStyleSheet("font : 12pt;");
	statusBarLabel.setText("<strong></strong>");

	mainMenu_graphicsScene = new MainMenuScene(this);
	connect(mainMenu_graphicsScene, &MainMenuScene::requestStart, this, &MainWindow::startGame);
	connect(mainMenu_graphicsScene, &MainMenuScene::requestQuit, this, &MainWindow::quitApp);
	connect(mainMenu_graphicsScene, SIGNAL(changeSoundEffectVolume(int)), this, SLOT(changeSoundEffectVolume(int)));
	connect(mainMenu_graphicsScene, SIGNAL(changeMusicVolume(int)), this, SLOT(changeMusicVolume(int)));

	ui->graphicsView->setScene(mainMenu_graphicsScene);
	setAttribute(Qt::WA_AlwaysShowToolTips, true);

	audioManager.start();
	audioManager.startMusic();

	settings = new QSettings("config.ini", QSettings::IniFormat, this);
	audioManager.changeSoundEffectVolume(settings->value("Volume/SoundEffect", 50).toInt());
	audioManager.changeMusicVolume(settings->value("Volume/Music", 25).toInt());

	mainMenu_graphicsScene->setSlidersValue(settings->value("Volume/Music", 25).toInt(), settings->value("Volume/SoundEffect", 50).toInt());
}

MainWindow::~MainWindow()
{
	audioManager.quit();
	audioManager.wait(2000);
	delete ui;
	delete mainMenu_graphicsScene;
	if(mainGame_graphicsScene!=nullptr){
		delete mainGame_graphicsScene;
	}
	delete settings;
}

void MainWindow::set_all_power_buttons_enabled(bool enabled)
{
	ui->button_place_power_1->setEnabled(enabled);
	ui->button_place_power_2->setEnabled(enabled);
	ui->button_place_power_3->setEnabled(enabled);
	ui->button_place_power_4->setEnabled(enabled);
	ui->button_place_power_5->setEnabled(enabled);
	ui->button_place_power_6->setEnabled(enabled);
	ui->button_place_power_7->setEnabled(enabled);
	ui->button_remove_power->setEnabled(enabled);
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
		connect(mainGame_graphicsScene, SIGNAL(gameMoneyUpdated(double)), this, SLOT(gameMoneyUpdated(double)));
		connect(mainGame_graphicsScene, SIGNAL(gameDemandsUpdated(double,int,double,double)), this, SLOT(gameDemandsUpdated(double,int,double,double)));
		connect(mainGame_graphicsScene, SIGNAL(gamePowerUpdated(double,double)), this, SLOT(gamePowerUpdated(double,double)));
		connect(mainGame_graphicsScene, SIGNAL(gamePollutionUpdated(int,int)), this, SLOT(gamePollutionUpdated(int,int)));
		connect(mainGame_graphicsScene, SIGNAL(playSoundEffect(SoundEffects)), &audioManager, SLOT(playSoundEffect(SoundEffects)));
		connect(mainGame_graphicsScene, SIGNAL(changeStatusBarMessage(QString)), this, SLOT(changeStatusBarMessage(QString)));

		ui->graphicsView->setScene(mainGame_graphicsScene);
		ui->graphicsView->setPanEnable(true);
		ui->graphicsView->setZoomEnbale(true);
		ui->graphicsView->fitInView(mainGame_graphicsScene->width()/4, mainGame_graphicsScene->height()/4, mainGame_graphicsScene->width()/2, mainGame_graphicsScene->height()/2, Qt::KeepAspectRatio);
		mainGame_graphicsScene->startGameLogic();

		ui->graphicsView->fitInView(0, 0, mainGame_graphicsScene->width()/2, mainGame_graphicsScene->height()/2, Qt::KeepAspectRatioByExpanding);
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
		mainGame_graphicsScene->enableBuildMode(Grid::Road::Type::TwoLanes);
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
		mainGame_graphicsScene->enableBuildMode(Grid::Road::Type::FourLanes);
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
		mainGame_graphicsScene->enableBuildMode(Grid::Road::Type::Highway);
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
		mainGame_graphicsScene->enableDestroyMode(Grid::Type::GridRoad);
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
		mainGame_graphicsScene->enableBuildMode(Grid::Zone::Type::Residential);
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
		mainGame_graphicsScene->enableBuildMode(Grid::Zone::Type::Commercial);
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
		mainGame_graphicsScene->enableBuildMode(Grid::Zone::Type::Industrial);
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
		mainGame_graphicsScene->enableBuildMode(Grid::Zone::Type::None);
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
		set_all_power_buttons_enabled(false);
		ui->button_place_power_1->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableBuildMode(Buildings::Service::Coal);
	}
	else{
		set_all_power_buttons_enabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}

void MainWindow::on_button_place_power_2_clicked(bool checked)
{
	if(checked){
		set_all_power_buttons_enabled(false);
		ui->button_place_power_2->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableBuildMode(Buildings::Service::Gas);
	}
	else{
		set_all_power_buttons_enabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}

void MainWindow::on_button_place_power_3_clicked(bool checked)
{
	if(checked){
		set_all_power_buttons_enabled(false);
		ui->button_place_power_3->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableBuildMode(Buildings::Service::Oil);
	}
	else{
		set_all_power_buttons_enabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}

void MainWindow::on_button_place_power_4_clicked(bool checked)
{
	if(checked){
		set_all_power_buttons_enabled(false);
		ui->button_place_power_4->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableBuildMode(Buildings::Service::Wind);
	}
	else{
		set_all_power_buttons_enabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}

void MainWindow::on_button_place_power_5_clicked(bool checked)
{
	if(checked){
		set_all_power_buttons_enabled(false);
		ui->button_place_power_5->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableBuildMode(Buildings::Service::SmallSolar);
	}
	else{
		set_all_power_buttons_enabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}

void MainWindow::on_button_place_power_6_clicked(bool checked)
{
	if(checked){
		set_all_power_buttons_enabled(false);
		ui->button_place_power_6->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableBuildMode(Buildings::Service::BigSolar);
	}
	else{
		set_all_power_buttons_enabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}

void MainWindow::on_button_place_power_7_clicked(bool checked)
{
	if(checked){
		set_all_power_buttons_enabled(false);
		ui->button_place_power_7->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableBuildMode(Buildings::Service::Nuclear);
	}
	else{
		set_all_power_buttons_enabled(true);
		ui->tabWidget_construction->setTabEnabled(0, true);
		ui->tabWidget_construction->setTabEnabled(1, true);
		mainGame_graphicsScene->disableBuildMode();
	}
}


void MainWindow::on_button_remove_power_clicked(bool checked)
{
	if(checked){
		set_all_power_buttons_enabled(false);
		ui->button_remove_power->setEnabled(true);
		ui->tabWidget_construction->setTabEnabled(0, false);
		ui->tabWidget_construction->setTabEnabled(1, false);
		mainGame_graphicsScene->enableDestroyMode(Grid::Type::GridBuilding);
	}
	else{
		set_all_power_buttons_enabled(true);
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
	if(mainGame_graphicsScene == nullptr){
		return;
	}

	if(index == 0){
		changeStatusBarMessage(statusBarMessage);
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

		if(ui->button_place_power_1->isChecked()){
			ui->button_place_power_1->setChecked(false);
			on_button_place_power_1_clicked(false);
		}
		if(ui->button_place_power_2->isChecked()){
			ui->button_place_power_2->setChecked(false);
			on_button_place_power_2_clicked(false);
		}
		if(ui->button_place_power_3->isChecked()){
			ui->button_place_power_3->setChecked(false);
			on_button_place_power_3_clicked(false);
		}
		if(ui->button_place_power_4->isChecked()){
			ui->button_place_power_4->setChecked(false);
			on_button_place_power_4_clicked(false);
		}
		if(ui->button_place_power_5->isChecked()){
			ui->button_place_power_5->setChecked(false);
			on_button_place_power_5_clicked(false);
		}
		if(ui->button_place_power_6->isChecked()){
			ui->button_place_power_6->setChecked(false);
			on_button_place_power_6_clicked(false);
		}
		if(ui->button_place_power_7->isChecked()){
			ui->button_place_power_7->setChecked(false);
			on_button_place_power_7_clicked(false);
		}
		if(ui->button_remove_power->isChecked()){
			ui->button_remove_power->setChecked(false);
			on_button_remove_power_clicked(false);
		}

		if(mainGame_graphicsScene!=nullptr){
			mainGame_graphicsScene->showZones(false);
		}
	}
	else if(index == 1){
		changeStatusBarMessage("<strong><font color=red>Mode construction : jeu en pause</font></strong>");
		mainGame_graphicsScene->setGameSpeed(0);
	}

	if(mainGame_graphicsScene!=nullptr){
		mainGame_graphicsScene->showZones(index == 1 && ui->tabWidget_construction->currentIndex()==1);
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

void MainWindow::gamePowerUpdated(double powerProduction, double powerConsumption)
{
	ui->label_electrical_production->setText("Production : "+QString::number(powerProduction, 'f', 2)+" MW");
	ui->label_electrical_consumption->setText("Consommation : "+QString::number(powerConsumption, 'f', 2)+" MW");
	if(powerProduction == 0){
		powerProduction = 1;
	}
	ui->progressBar_electrical->setMaximum(powerProduction);
	ui->progressBar_electrical->setValue(powerConsumption);
}

void MainWindow::gameMoneyUpdated(double money)
{
	int divider;
	QString suffix;
	if(abs(money) >= 1e9){
		divider = 1e9;
		suffix = " Md€";
	}
	else if(abs(money) >= 1e6){
		divider = 1e6;
		suffix = " M€";
	}
	else if(abs(money) >= 1e3){
		divider = 1e3;
		suffix = " k€";
	}
	else{
		divider = 1;
		suffix = " €";
	}

	int variationDivider;
	QString variationSuffix;
	if(abs(money - this->money) >= 1e9){
		variationDivider = 1e9;
		variationSuffix = " Md€";
	}
	else if(abs(money - this->money) >= 1e6){
		variationDivider = 1e6;
		variationSuffix = " M€";
	}
	else if(abs(money - this->money) >= 1e3){
		variationDivider = 1e3;
		variationSuffix = " k€";
	}
	else{
		variationDivider = 1;
		variationSuffix = " €";
	}

	QPalette palette = ui->label_money_variation->palette();
	if(money-this->money > 0){ //Money has increased
		ui->label_money_variation->setText("+"+QString::number((money - this->money)/variationDivider, 'f', 2)+variationSuffix);
		palette.setColor(ui->label_money_variation->foregroundRole(), Qt::green);

	}
	else if(money-this->money == 0){
		ui->label_money_variation->setText("");
	}
	else{ //Money has decreased
		ui->label_money_variation->setText("-"+QString::number((this->money - money)/variationDivider, 'f', 2)+variationSuffix);
		palette.setColor(ui->label_money_variation->foregroundRole(), Qt::red);
	}

	ui->label_money_variation->setPalette(palette);
	this->money = money;
	ui->label_money->setText("Argent : "+QString::number(money/divider, 'f', 2)+suffix);
}

void MainWindow::gamePollutionUpdated(int pollution, int maxPollution)
{
	if(maxPollution == 0){
		maxPollution = 1;
	}

	QColor color(pollution*255/maxPollution, (maxPollution-pollution)*255/maxPollution, 0, 255);
	ui->progressBar_pollution_emissions->setStyleSheet("QProgressBar::chunk{background-color: "+color.name()+"}");
	ui->progressBar_pollution_emissions->setMaximum(maxPollution);
	ui->progressBar_pollution_emissions->setValue(pollution);
	ui->label_pollution_emissions->setText("Emissions : "+QString::number(pollution));
	ui->label_pollution_emissions_per_hab->setText("Emissions par hab. : "+QString::number((double)pollution/(double)residents, 'f', 2));
}

void MainWindow::changeStatusBarMessage(QString newMessage)
{
	if(ui->tabWidget_main->currentIndex() != 1){
		statusBarMessage = newMessage;
	}

	statusBarLabel.setText(newMessage);
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

void MainWindow::changeSoundEffectVolume(int volume)
{
	audioManager.changeSoundEffectVolume(volume);
	audioManager.playSoundEffect(RoadPlacement);
	settings->setValue("Volume/SoundEffect", volume);
}

void MainWindow::changeMusicVolume(int volume)
{
	audioManager.changeMusicVolume(volume);
	settings->setValue("Volume/Music", volume);
}

