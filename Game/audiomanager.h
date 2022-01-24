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

//Classe représentant le gestionnaire de son
class AudioManager : public QThread
{
	Q_OBJECT
public:
	AudioManager(QObject *parent = nullptr);
	~AudioManager();

	void startMusic();							//Début la lecture de la musique
	void changeSoundEffectVolume(int volume);	//Changement du volume des effets sonore
	void changeMusicVolume(int volume);			//Changement du volume de la musique

public slots:
	void playSoundEffect(SoundEffects soundEffect);	//Jouer un son

private:
	QMediaPlayer *musicPlayer;	//Lecteur de musique
	QAudioOutput *audioOutput;	//Sortie du lecteur de musique
	QList<QUrl> musicPlaylist;	//Playlist de musique

	int musicIndex;				//Indice dans la playlist de la musique actuellement lue

	QMap<SoundEffects, QSoundEffect*> soundEffects;	//Liste des effets sonores

	/* La différence entre un QSoundEffect et un QMediaPlayer
	 * est que le QSoundEffect est préchargé en mémoire alors que le
	 * QMediaPlayer "stream" la musique depuis le stockage.
	 * Le QSoundEffect est donc plus réactif que le QMediaPlayer
	 * mais aussi plus demandant en mémoire.
	 */

	void loadMusics();				//Chargement des musiques
	void loadSoundEffects();		//Chargement des effets sonores

private slots:
	void musicStatusChanged(QMediaPlayer::MediaStatus status);	//Lorsque que le lecteur de musique change d'état
};

#endif // AUDIOMANAGER_H
