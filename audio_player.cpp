#include <QDebug>

#include "audio_player.h"

//#define MEDIAPLAYER

AudioPlayer::AudioPlayer() : soundEffect(nullptr)
{
}

void AudioPlayer::play(QString const& path)
{
    qDebug() << __FUNCSIG__;
    qDebug() << path;

#ifdef MEDIAPLAYER
    if (mediaPlayer.state() != QMediaPlayer::StoppedState) {
        return;
    }

    mediaPlayer.setMedia(QUrl::fromLocalFile(path));
    mediaPlayer.play();
#else
    // Must reset sound effect to deal with sample rate changes
    delete soundEffect;
    soundEffect = new QSoundEffect;
    soundEffect->setSource(QUrl::fromLocalFile(path));
    soundEffect->play();
#endif
}

void AudioPlayer::stop()
{
#ifdef MEDIAPLAYER
    mediaPlayer.stop();
#else
    if (soundEffect) {
        soundEffect->stop();
    }
#endif
}

