#include <QCoreApplication>
#include <QDebug>

#include "RtspClient.h"
#include "RtpReceiver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RtspClient client;

    QString url = "rtsp://172.25.32.1:8554/live";

    // ================= 错误 =================
    QObject::connect(&client, &RtspClient::errorOccurred,
        [](const QString &err)
    {
        qDebug() << "ERROR:" << err;
    });

    client.start(url);

    return a.exec();
}