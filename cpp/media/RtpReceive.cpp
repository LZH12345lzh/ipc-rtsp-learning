#include "RtpReceiver.h"
#include <QDebug>

RtpReceiver::RtpReceiver(QObject *parent)
    : QObject(parent)
{
}

// ===================== start =====================
void RtpReceiver::start(const MediaSession &session)
{
    Q_UNUSED(session)

    started = true;

    qDebug() << "RTP Receiver (TCP) started";
}

// ===================== stop =====================
void RtpReceiver::stop()
{
    started = false;
    buffer.clear();

    qDebug() << "RTP Receiver stopped";
}

void RtpReceiver::onRtpPacket(const QByteArray &packet)
{
    parseRtp(packet);
}

void RtpReceiver::parseRtp(const QByteArray &rtp)
{
    if (rtp.size() < 12)
        return;

    quint8 payloadType = rtp[1] & 0x7F;
    quint16 seq = (quint8)rtp[2] << 8 | (quint8)rtp[3];

    qDebug() << "RTP packet:"
             << "PT=" << payloadType
             << "SEQ=" << seq
             << "SIZE=" << rtp.size();
}
