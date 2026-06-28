#pragma once
#include <QString>
#include <QList>

struct MediaTrack
{
    QString media;
    QString codec;
    int payloadType = 0;
    int clockRate = 0;
    QString control;
};

class MediaSession
{
public:
    QList<MediaTrack> tracks;
};