#include <QDebug>

#include "audio_player.h"

//#define MEDIAPLAYER

AudioPlayer::AudioPlayer(bool loop) : mediaPlayer(nullptr), soundEffect(nullptr), loop(loop)
{
}

void AudioPlayer::play(QString const& path)
{
    qDebug() << __FUNCSIG__;
    qDebug() << path;

#ifdef MEDIAPLAYER
    delete mediaPlayer;
    mediaPlayer = new QMediaPlayer;
    mediaPlayer->setMedia(QUrl::fromLocalFile(path));
    mediaPlayer->play();
#else
    // Must reset sound effect to deal with sample rate changes
    delete soundEffect;
    soundEffect = new QSoundEffect;
    soundEffect->setSource(QUrl::fromLocalFile(path));
    setLoop(this->loop);
    soundEffect->play();
#endif
}

void AudioPlayer::stop()
{
#ifdef MEDIAPLAYER
    if (mediaPlayer) {
        mediaPlayer->stop();
    }
#else
    if (soundEffect) {
        soundEffect->stop();
    }
#endif
}

void AudioPlayer::setLoop(bool loop)
{
    this->loop = loop;
    if (soundEffect) {
        soundEffect->setLoopCount(this->loop ? QSoundEffect::Infinite : 0);
    }
}


