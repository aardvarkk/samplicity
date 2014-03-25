#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QFile>
#include <QMediaPlayer>
#include <QSoundEffect>

class AudioPlayer
{
public:
    AudioPlayer();

    void play(QString const& path);
    void stop();

private:
    QMediaPlayer mediaPlayer;
    QSoundEffect* soundEffect;
};

#endif // AUDIO_PLAYER_H
