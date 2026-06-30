#include "RtspClient.h"
#include "SDPParser.h"

#include <QUrl>
#include <QDebug>
#include <QTimer>

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

// ===================== 启动 =====================
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

    sendOptions();
}

// ===================== 收包 =====================
void RtspClient::onReadyRead()
{
    buffer += socket->readAll();

    while (true)
    {
        if (buffer.isEmpty())
            break;

        // ================= RTP over TCP =================
        if ((unsigned char)buffer[0] == '$')
        {
            if (!processRtpInterleaved(buffer))
                break;
            continue;
        }

        // ================= RTSP Response =================
        if (!buffer.contains("\r\n\r\n"))
            break;

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
}

bool RtspClient::processRtpInterleaved(const QByteArray &data)
{
    if (data.size() < 4)
        return false;

    // $ | channel | len (2 bytes)
    quint16 len = (quint8)data[2] << 8 | (quint8)data[3];

    if (data.size() < len + 4)
        return false;

    QByteArray rtp = data.mid(4, len);

    buffer.remove(0, len + 4);

    rtpReceiver.onRtpPacket(rtp);

    return true;
}

// ===================== 发送请求 =====================
void RtspClient::sendRequest(const RtspRequest &req)
{
    QByteArray data = req.toByteArray();

    qDebug().noquote() << ">>> RTSP Request:\n" << data;

    socket->write(data);
}

// ===================== OPTIONS =====================
void RtspClient::sendOptions()
{
    RtspRequest req;
    req.setMethod(RtspRequest::OPTIONS);
    req.setUrl(url);
    req.setCSeq(cseq++);

    sendRequest(req);

    state = State::OptionsSent;
}

// ===================== DESCRIBE =====================
void RtspClient::sendDescribe()
{
    RtspRequest req;
    req.setMethod(RtspRequest::DESCRIBE);
    req.setUrl(url);
    req.setCSeq(cseq++);

    sendRequest(req);

    state = State::DescribeSent;
}

// ===================== SETUP =====================
void RtspClient::sendSetup(MediaTrack *track)
{
    QString setupUrl = url;

    if (!setupUrl.endsWith('/'))
        setupUrl += '/';

    setupUrl += track->control;

    RtspRequest req;
    req.setMethod(RtspRequest::SETUP);
    req.setUrl(setupUrl);
    req.setCSeq(cseq++);

    req.setClientPorts(track->clientRtpPort,
                        track->clientRtcpPort);

    // qDebug() << ">>> SETUP:" << setupUrl;

    sendRequest(req);

    state = State::SetupSent;
}

// ===================== PLAY =====================
void RtspClient::sendPlay(QString session)
{
    RtspRequest req;
    req.setMethod(RtspRequest::PLAY);
    req.setUrl(url);
    req.setCSeq(cseq++);
    QString id = session;
    if (session.contains(';'))
    {

        id = session.section(';', 0, 0);
    }
    req.setSessionId(id);
    // req.addHeader("Range: npt=0.000-");
    sendRequest(req);
}

// ===================== Response分发 =====================
void RtspClient::handleResponse(const RtspResponse &resp)
{
    switch (state)
    {
    case State::OptionsSent:
        onOptionsResponse(resp);
        break;

    case State::DescribeSent:
        onDescribeResponse(resp);
        break;

    case State::SetupSent:
        onSetupResponse(resp);
        break;

    default:
        break;
    }
}

// ===================== OPTIONS response =====================
void RtspClient::onOptionsResponse(const RtspResponse &resp)
{
    qDebug() << "<<< OPTIONS Response:" << resp.statusCode();

    if (resp.statusCode() != 200)
    {
        emit errorOccurred("OPTIONS failed");
        state = State::Error;
        return;
    }

    sendDescribe();
}

// ===================== DESCRIBE response =====================
void RtspClient::onDescribeResponse(const RtspResponse &resp)
{
    qDebug() << "<<< DESCRIBE Response:" << resp.statusCode();

    if (resp.statusCode() != 200)
    {
        emit errorOccurred("DESCRIBE failed");
        state = State::Error;
        return;
    }

    qDebug().noquote() << "<<< SDP:\n" << resp.body();

    // 解析 SDP
    mediaSession = SDPParser::parse(resp.body());

    MediaTrack *track = mediaSession.firstTrack("video");
    if (!track)
    {
        emit errorOccurred("No video track");
        state = State::Error;
        return;
    }

    sendSetup(track);
}

// ===================== SETUP response =====================
void RtspClient::onSetupResponse(const RtspResponse &resp)
{
    qDebug() << "<<< SETUP Response:" << resp.statusCode();

    if (resp.statusCode() != 200)
    {
        emit errorOccurred("SETUP failed");
        state = State::Error;
        return;
    }

    qDebug().noquote() << "<<< SETUP OK:\n" << resp.body();

    state = State::Ready;
    rtpReceiver.start(mediaSession);
    sendPlay(resp.header("Session"));

    qDebug() << "RTSP Ready (control plane finished)";
}

// ===================== 错误处理 =====================
void RtspClient::onError(QAbstractSocket::SocketError)
{
    state = State::Error;

    qDebug() << "Socket Error:" << socket->errorString();

    emit errorOccurred(socket->errorString());
}