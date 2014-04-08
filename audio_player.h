#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QFile>

//#define MEDIAPLAYER
#ifdef MEDIAPLAYER
#include <QMediaPlayer>
#endif

//#define SOUNDEFFECT
#ifdef SOUNDEFFECT
#include <QSoundEffect>
#endif

#define IRRKLANG
#ifdef IRRKLANG
#include <irrKlang.h>
#endif

#include "error_code.h"

class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    AudioPlayer(bool loop = false);
    ~AudioPlayer();

    ErrorCode play(QString const& path);
    void setLoop(bool loop);
    void stop();

public slots:
    void setVolume(int volume);

private:

#ifdef MEDIAPLAYER
    QMediaPlayer* mediaPlayer;
#endif

#ifdef SOUNDEFFECT
    QSoundEffect* soundEffect;
#endif

#ifdef IRRKLANG
    irrklang::ISoundEngine* soundEngine;
#endif

    bool loop;
};

#endif // AUDIO_PLAYER_H
