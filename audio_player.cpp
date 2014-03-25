#include <QDebug>

#include "audio_player.h"

AudioPlayer::AudioPlayer()
{
}

void AudioPlayer::play(QString const& path)
{
    qDebug() << __FUNCSIG__;
    qDebug() << path;

    soundEffect.setSource(QUrl::fromLocalFile(path));
    soundEffect.play();
}

void AudioPlayer::stop()
{
    soundEffect.stop();
}

