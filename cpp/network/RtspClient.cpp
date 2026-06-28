#include "RtspClient.h"
#include "SDPParser.h"

#include <QUrl>
#include <QDebug>

// ===================== 构造 =====================
RtspClient::RtspClient(QObject *parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected,
            this, &RtspClient::onConnected);

    connect(socket, &QTcpSocket::readyRead,
            this, &RtspClient::onReadyRead);

    connect(socket,
            static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this,
            &RtspClient::onError);
}

// ===================== 启动连接 =====================
void RtspClient::start(const QString &u)
{
    url = u;
    buffer.clear();

    state = State::Connecting;

    QUrl urlObj(url);

    qDebug() << "Connecting:" << url;

    socket->connectToHost(urlObj.host(),
                          urlObj.port(8554));
}

// ===================== TCP Connected =====================
void RtspClient::onConnected()
{
    qDebug() << "TCP Connected";

    state = State::Connected;

    // ---------- OPTIONS ----------
    RtspRequest req;
    req.setMethod(RtspRequest::OPTIONS);
    req.setUrl(url);
    req.setCSeq(cseq++);

    sendRequest(req);

    state = State::OptionsSent;
}

// ===================== 收到数据 =====================
void RtspClient::onReadyRead()
{
    buffer += socket->readAll();

    // RTSP 响应结束标志
    if (!buffer.contains("\r\n\r\n"))
        return;

    RtspResponse resp;
    if (!resp.parse(buffer))
    {
        emit errorOccurred("RTSP parse failed");
        buffer.clear();
        return;
    }

    buffer.clear();

    handleResponse(resp);
}

// ===================== 发送请求 =====================
void RtspClient::sendRequest(const RtspRequest &req)
{
    QByteArray data = req.toByteArray();

    qDebug().noquote() << ">>> RTSP Request:\n" << data;

    socket->write(data);
}

// ===================== 响应处理（状态机核心） =====================
void RtspClient::handleResponse(const RtspResponse &resp)
{
    qDebug() << "<<< RTSP Response:"
             << resp.statusCode();

    // ================= OPTIONS 返回 =================
    if (state == State::OptionsSent)
    {
        if (resp.statusCode() != 200)
        {
            emit errorOccurred("OPTIONS failed");
            state = State::Error;
            return;
        }

        // ---------- DESCRIBE ----------
        RtspRequest req;
        req.setMethod(RtspRequest::DESCRIBE);
        req.setUrl(url);
        req.setCSeq(cseq++);

        sendRequest(req);

        state = State::DescribeSent;
        return;
    }

    // ================= DESCRIBE 返回（SDP） =================
    if (state == State::DescribeSent)
    {
        if (resp.statusCode() != 200)
        {
            emit errorOccurred("DESCRIBE failed");
            state = State::Error;
            return;
        }

        qDebug().noquote() << "<<< SDP:\n" << resp.body();

        MediaSession session = SDPParser::parse(resp.body());

        state = State::Ready;

        emit sdpParsed(session);

        return;
    }
}

// ===================== 错误处理 =====================
void RtspClient::onError(QAbstractSocket::SocketError)
{
    state = State::Error;

    qDebug() << "Socket Error:" << socket->errorString();

    emit errorOccurred(socket->errorString());
}