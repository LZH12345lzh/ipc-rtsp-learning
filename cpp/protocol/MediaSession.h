#pragma once

#include <QString>
#include <QList>

struct MediaTrack
{
    // ===== SDP =====
    QString media;          // video / audio
    QString codec;          // H265
    int payloadType = 0;
    int clockRate = 90000;
    QString control;        // trackID=0

    // ===== RTP Client =====
    quint16 clientRtpPort = 5000;
    quint16 clientRtcpPort = 5001;

    // ===== RTP Server =====
    quint16 serverRtpPort = 0;
    quint16 serverRtcpPort = 0;

    // ===== RTSP =====
    QString sessionId;
};

class MediaSession
{
public:
    QList<MediaTrack> tracks;

    // ===================== 新接口 =====================
    MediaTrack* firstTrack(const QString &mediaType)
    {
        for (auto &t : tracks)
        {
            if (t.media == mediaType)
                return &t;
        }
        return nullptr;
    }
};