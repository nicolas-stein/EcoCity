#include "audiomanager.h"

AudioManager::AudioManager(QObject *parent): QThread {parent}
{
	moveToThread(this);					//Tous les slots s'éxecuteront dans un thread séparé afin de pas ralentir le thread principal (gérant la partie graphique)
	musicPlayer = new QMediaPlayer();	//On créer le lecteur de musique qu'on déplace dans le thread séparé
	musicPlayer->moveToThread(this);
	audioOutput = new QAudioOutput();	//On créer la sortie audio qu'on déplace dans le thread séparé
	audioOutput->moveToThread(this);

	musicPlayer->setAudioOutput(audioOutput);
	audioOutput->setVolume(0.25);

	loadMusics();
	loadSoundEffects();

	connect(musicPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(musicStatusChanged(QMediaPlayer::MediaStatus)));
}

AudioManager::~AudioManager()
{
	//On supprime tout les sons chargés et autres variables allouée dynamiquement
	foreach(QSoundEffect *soundEffect, soundEffects){
		delete soundEffect;
	}
	musicPlayer->stop();
	delete musicPlayer;
	delete audioOutput;
}

void AudioManager::startMusic()
{
	//Si la musique n'est pas déjà en cours de lecture, on lit une musique au hasard dans la playlist
	if(musicPlayer->playbackState() != QMediaPlayer::PlayingState){
		musicIndex = QRandomGenerator::global()->generate() % musicPlaylist.size();
		musicPlayer->setSource(musicPlaylist.at(musicIndex));
		musicPlayer->play();
	}
}

void AudioManager::playSoundEffect(SoundEffects soundEffect)
{
	//Si le son en question n'est pas déjà en cours de lecture, on le joue
	if(!soundEffects.value(soundEffect)->isPlaying()){
		soundEffects.value(soundEffect)->play();
	}
}

void AudioManager::changeSoundEffectVolume(int volume)
{
	double vol = (double) volume /100;
	foreach(QSoundEffect *soundEffect, soundEffects){
		soundEffect->setVolume(vol);
	}
}

void AudioManager::changeMusicVolume(int volume)
{
	audioOutput->setVolume((double)volume/100);
}

void AudioManager::loadMusics()
{
	for(int i=1;i<=4;i++){
		musicPlaylist.append(QUrl("qrc:/sounds/music/music_"+QString::number(i)));
	}
}

void AudioManager::loadSoundEffects()
{
	QSoundEffect *soundEffect = new QSoundEffect();
	soundEffect->moveToThread(this);soundEffect->setLoopCount(1);
	soundEffect->setSource(QUrl("qrc:/sounds/sound_effect/placeRoad"));
	soundEffects.insert(RoadPlacement, soundEffect);

	soundEffect = new QSoundEffect();
	soundEffect->moveToThread(this); soundEffect->setLoopCount(1);
	soundEffect->setSource(QUrl("qrc:/sounds/sound_effect/placeZone"));
	soundEffects.insert(ZonePlacement, soundEffect);

	soundEffect = new QSoundEffect();
	soundEffect->moveToThread(this); soundEffect->setLoopCount(1);
	soundEffect->setSource(QUrl("qrc:/sounds/sound_effect/placeBuilding"));
	soundEffects.insert(BuildingPlacement, soundEffect);

	soundEffect = new QSoundEffect();
	soundEffect->moveToThread(this); soundEffect->setLoopCount(1);
	soundEffect->setSource(QUrl("qrc:/sounds/sound_effect/destroyLong"));
	soundEffects.insert(DestroyLong, soundEffect);

	soundEffect = new QSoundEffect();
	soundEffect->moveToThread(this); soundEffect->setLoopCount(1);
	soundEffect->setSource(QUrl("qrc:/sounds/sound_effect/destroyShort"));
	soundEffects.insert(DestroyShort, soundEffect);
}

void AudioManager::musicStatusChanged(QMediaPlayer::MediaStatus status)
{
	//Si la musique est terminée, on joue au hasard une autre musique
	if(status == QMediaPlayer::EndOfMedia){
		int currentIndex = musicIndex;
		while(musicIndex == currentIndex){
			musicIndex = QRandomGenerator::global()->generate() % musicPlaylist.size();
		}
		musicPlayer->setSource(musicPlaylist.at(musicIndex));
		musicPlayer->play();
	}
}
