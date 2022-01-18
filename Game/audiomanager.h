#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "constants.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QMap>
#include <QList>
#include <QUrl>
#include <QRandomGenerator>
#include <QThread>


class AudioManager : public QThread
{
	Q_OBJECT
public:
	AudioManager();
	~AudioManager();

	void startMusic();

public slots:
	void playSoundEffect(SoundEffects soundEffect);

private:
	QMediaPlayer *musicPlayer;
	QAudioOutput *audioOutput;
	QList<QUrl> musicPlaylist;

	int musicIndex;

	QMap<SoundEffects, QSoundEffect*> soundEffects;

	void loadMusics();
	void loadSoundEffects();

private slots:
	void musicStatusChanged(QMediaPlayer::MediaStatus status);
};

#endif // AUDIOMANAGER_H
