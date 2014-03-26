#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QFile>
#include <QMediaPlayer>
#include <QSoundEffect>

class AudioPlayer
{
public:
    AudioPlayer(bool loop = false);

    void play(QString const& path);
    void setLoop(bool loop);
    void stop();

private:
    QMediaPlayer* mediaPlayer;
    QSoundEffect* soundEffect;
    bool loop;
};

#endif // AUDIO_PLAYER_H
