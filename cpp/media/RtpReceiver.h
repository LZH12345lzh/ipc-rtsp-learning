#pragma once

#include <QObject>
#include <QTcpSocket>

#include "MediaSession.h"

class RtpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit RtpReceiver(QObject *parent = nullptr);
    void onRtpPacket(const QByteArray &packet);

public slots:
    void start(const MediaSession &session);
    void stop();

private:
    void parseRtp(const QByteArray &rtp);

private:
    QByteArray buffer;

    bool started = false;
};