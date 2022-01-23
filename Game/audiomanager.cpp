#include "audiomanager.h"

AudioManager::AudioManager(QObject *parent): QThread {parent}
{
	moveToThread(this);
	musicPlayer = new QMediaPlayer();
	musicPlayer->moveToThread(this);
	audioOutput = new QAudioOutput();
	audioOutput->moveToThread(this);

	musicPlayer->setAudioOutput(audioOutput);
	audioOutput->setVolume(0.25);

	loadMusics();
	loadSoundEffects();

	connect(musicPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(musicStatusChanged(QMediaPlayer::MediaStatus)));
}

AudioManager::~AudioManager()
{
	foreach(QSoundEffect *soundEffect, soundEffects){
		delete soundEffect;
	}
	musicPlayer->stop();
	delete musicPlayer;
	delete audioOutput;
}

void AudioManager::startMusic()
{
	if(musicPlayer->playbackState() != QMediaPlayer::PlayingState){
		musicIndex = QRandomGenerator::global()->generate() % musicPlaylist.size();
		musicPlayer->setSource(musicPlaylist.at(musicIndex));
		musicPlayer->play();
	}
}

void AudioManager::playSoundEffect(SoundEffects soundEffect)
{
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
	if(status == QMediaPlayer::EndOfMedia){
		int currentIndex = musicIndex;
		while(musicIndex == currentIndex){
			musicIndex = QRandomGenerator::global()->generate() % musicPlaylist.size();
		}
		musicPlayer->setSource(musicPlaylist.at(musicIndex));
		musicPlayer->play();
	}
}
