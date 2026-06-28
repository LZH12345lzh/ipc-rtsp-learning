#include <QCoreApplication>
#include <QDebug>

#include "RtspClient.h"
#include "RtspRequest.h"
#include "RtspResponse.h"
#include "SDPParser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RtspClient client;

    QString url = "rtsp://172.25.32.1:8554/live";

    client.start(url);

    QObject::connect(&client, &RtspClient::sdpParsed,
        [](const MediaSession &session)
    {
        qDebug() << "==== SDP READY ====";
        qDebug() << "Track count:" << session.tracks.size();
        qDebug() << "Codec:" << session.tracks[0].codec;
        qDebug() << "Payload:" << session.tracks[0].payloadType;
    });

    return a.exec();
}