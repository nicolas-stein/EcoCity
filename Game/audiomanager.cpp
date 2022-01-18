#include "audiomanager.h"

AudioManager::AudioManager()
{
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
	delete soundEffects.value(RoadPlacement);
	delete soundEffects.value(ZonePlacement);
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

void AudioManager::loadMusics()
{
	for(int i=1;i<=4;i++){
		musicPlaylist.append(QUrl("qrc:/sounds/music/music_"+QString::number(i)));
	}
}

void AudioManager::loadSoundEffects()
{
	QSoundEffect *soundEffect = new QSoundEffect();
	soundEffect->moveToThread(this);soundEffect->setLoopCount(1); soundEffect->setVolume(0.5);
	soundEffect->setSource(QUrl("qrc:/sounds/sound_effect/placeRoad"));
	soundEffects.insert(RoadPlacement, soundEffect);

	soundEffect = new QSoundEffect();
	soundEffect->moveToThread(this); soundEffect->setLoopCount(1); soundEffect->setVolume(0.5);
	soundEffect->setSource(QUrl("qrc:/sounds/sound_effect/placeZone"));
	soundEffects.insert(ZonePlacement, soundEffect);
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
