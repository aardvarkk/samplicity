#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QFile>
#include <QSoundEffect>

class AudioPlayer
{
public:
    AudioPlayer();

    void play(QString const& path);
    void stop();

private:
    QSoundEffect soundEffect;
};

#endif // AUDIO_PLAYER_H
