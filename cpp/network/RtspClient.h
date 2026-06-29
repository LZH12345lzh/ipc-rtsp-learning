#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>

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
    void rtspReady();
    void errorOccurred(const QString &err);

private slots:
    void onConnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError);

private:
    enum State
    {
        Connecting,
        Connected,
        OptionsSent,
        DescribeSent,
        SetupSent,
        Ready,
        Error
    };

private:
    // ===== 网络 =====
    QTcpSocket *socket = nullptr;
    QString url;
    QByteArray buffer;

    // ===== 状态机 =====
    State state = Connecting;
    int cseq = 1;

    // ===== 媒体会话 =====
    MediaSession mediaSession;

private:
    // ===== 请求发送 =====
    void sendRequest(const RtspRequest &req);

    void sendOptions();
    void sendDescribe();
    void sendSetup(MediaTrack *track);

    // ===== 响应处理 =====
    void handleResponse(const RtspResponse &resp);

    void onOptionsResponse(const RtspResponse &resp);
    void onDescribeResponse(const RtspResponse &resp);
    void onSetupResponse(const RtspResponse &resp);
};