#include "RtspRequest.h"
#include <QDebug>

void RtspRequest::setMethod(Method m) { method = m; }
void RtspRequest::setUrl(const QString &u) { url = u; }
void RtspRequest::setCSeq(int s) { cseq = s; }

void RtspRequest::setClientPorts(quint16 rtp, quint16 rtcp)
{
    clientRtpPort = rtp;
    clientRtcpPort = rtcp;
}

void RtspRequest::setSessionId(const QString &id)
{
    sessionId = id;
}

QByteArray RtspRequest::toByteArray() const
{
    QString methodStr;

    switch (method)
    {
    case OPTIONS:  methodStr = "OPTIONS";  break;
    case DESCRIBE: methodStr = "DESCRIBE"; break;
    case SETUP:    methodStr = "SETUP";    break;
    case PLAY:     methodStr = "PLAY";     break;
    case TEARDOWN: methodStr = "TEARDOWN"; break;
    }

    QString req;

    req += methodStr + " " + url + " RTSP/1.0\r\n";

    // DESCRIBE
    if (method == DESCRIBE)
    {
        req += "Accept: application/sdp\r\n";
    }

    // SETUP
    if (method == SETUP)
    {
        // tcp
        req += "Transport: RTP/AVP/TCP;unicast;interleaved=0-1\r\n";
    }

    req += "CSeq: " + QString::number(cseq) + "\r\n";
    req += "User-Agent: QtRtspClient\r\n";
    // PLAY / TEARDOWN
    if (method == PLAY)
    {
        req += "Range: npt=0.000-\r\n";

        if (!sessionId.isEmpty())
        {
            req += "Session: " + sessionId + "\r\n";
        }
        else 
        {
            qDebug() << "sessionId is null";
        }
            
    }

    req += "\r\n";
    return req.toUtf8();
}