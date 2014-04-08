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

AudioPlayer::~AudioPlayer()
{
#ifdef IRRKLANG
    delete soundEngine;
#endif
}

ErrorCode AudioPlayer::play(QString const& path)
{
    qDebug() << __FUNCSIG__;
    qDebug() << path;

    if (!QFile::exists(path)) {
        return ErrorCode(ErrorCode::FileNotFound, path);
    }

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
    soundEngine->stopAllSounds();
    soundEngine->play2D(path.toLocal8Bit().constData(), this->loop);
#endif

    return ErrorCode(ErrorCode::Success);
}

void AudioPlayer::setVolume(int volume)
{
    volume = qBound(0, volume, 99);

#ifdef IRRKLANG
    soundEngine->setSoundVolume(static_cast<irrklang::ik_f32>(volume) / 99);
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

#ifdef IRRKLANG
    if (!this->loop) {
        soundEngine->stopAllSounds();
    }
#endif
}
