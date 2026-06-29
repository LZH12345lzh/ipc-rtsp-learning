#pragma once
#include <QString>

class RtspRequest
{
public:
    enum Method {
        OPTIONS,
        DESCRIBE,
        SETUP,
        PLAY,
        TEARDOWN
    };

    void setMethod(Method m);
    void setUrl(const QString &url);
    void setCSeq(int seq);

    void setClientPorts(quint16 rtp, quint16 rtcp);
    void setSessionId(const QString &id);

    QByteArray toByteArray() const;

private:
    Method method;
    QString url;
    int cseq = 1;

    quint16 clientRtpPort = 0;
    quint16 clientRtcpPort = 0;

    QString sessionId;
};