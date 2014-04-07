#include <QDebug>

#include "audio_player.h"

AudioPlayer::AudioPlayer(bool loop) : loop(loop)
{
#ifdef MEDIAPLAYER
    mediaPlayer = nullptr;
#endif
#ifdef SOUNDEFFECT
    soundEffect = nullptr;
#endif
#ifdef IRRKLANG
    soundEngine = irrklang::createIrrKlangDevice();
#endif
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
#endif

#ifdef SOUNDEFFECT
    // Must reset sound effect to deal with sample rate changes
    delete soundEffect;
    soundEffect = new QSoundEffect;
    soundEffect->setSource(QUrl::fromLocalFile(path));
    setLoop(this->loop);
    soundEffect->play();
#endif

#ifdef IRRKLANG
    soundEngine->play2D(path.toLocal8Bit().constData());
#endif
}

void AudioPlayer::stop()
{
#ifdef MEDIAPLAYER
    if (mediaPlayer) {
        mediaPlayer->stop();
    }
#endif

#ifdef SOUNDEFFECT
    if (soundEffect) {
        soundEffect->stop();
    }
#endif
}

void AudioPlayer::setLoop(bool loop)
{
    this->loop = loop;

#ifdef SOUNDEFFECT
    if (soundEffect) {
        soundEffect->setLoopCount(this->loop ? QSoundEffect::Infinite : 0);
    }
#endif
}


