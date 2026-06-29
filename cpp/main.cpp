#include <QCoreApplication>
#include <QDebug>

#include "RtspClient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RtspClient client;

    QString url = "rtsp://172.25.32.1:8554/live";

    // ================= RTSP Ready（主事件） =================
    QObject::connect(&client, &RtspClient::rtspReady,
        []( )
    {
        qDebug() << "==== RTSP READY ====";
        qDebug() << "Control plane finished (OPTIONS/DESCRIBE/SETUP)";
    });

    // ================= 错误 =================
    QObject::connect(&client, &RtspClient::errorOccurred,
        [](const QString &err)
    {
        qDebug() << "ERROR:" << err;
    });

    client.start(url);

    return a.exec();
}