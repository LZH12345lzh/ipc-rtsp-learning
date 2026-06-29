#include "RtspRequest.h"

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
    req += "CSeq: " + QString::number(cseq) + "\r\n";
    req += "User-Agent: QtRtspClient\r\n";

    // DESCRIBE
    if (method == DESCRIBE)
    {
        req += "Accept: application/sdp\r\n";
    }

    // SETUP
    if (method == SETUP)
    {
        req += QString(
            "Transport: RTP/AVP;unicast;"
            "client_port=%1-%2\r\n")
            .arg(clientRtpPort)
            .arg(clientRtcpPort);
    }

    // PLAY / TEARDOWN
    if (method == PLAY || method == TEARDOWN)
    {
        if (!sessionId.isEmpty())
        {
            req += "Session: " + sessionId + "\r\n";
        }
    }

    req += "\r\n";
    return req.toUtf8();
}