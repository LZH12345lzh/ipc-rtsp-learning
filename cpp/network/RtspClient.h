#pragma once

#include <QObject>
#include <QTcpSocket>
#include "RtspRequest.h"
#include "RtspResponse.h"
#include "MediaSession.h"

class RtspClient : public QObject
{
    Q_OBJECT

public:
    explicit RtspClient(QObject *parent = nullptr);

    void start(const QString &url);

signals:
    void errorOccurred(QString err);
    void sdpParsed(MediaSession session);

private slots:
    void onConnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError);

private:
    void sendRequest(const RtspRequest &req);
    void handleResponse(const RtspResponse &resp);

private:
    QTcpSocket *socket;

    QString url;
    QByteArray buffer;

    int cseq = 1;

    enum class State {
        Idle,
        Connecting,
        Connected,
        OptionsSent,
        DescribeSent,
        Ready,
        Error
    };

    State state = State::Idle;
};