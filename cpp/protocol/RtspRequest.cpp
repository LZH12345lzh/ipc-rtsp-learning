#include "RtspRequest.h"

void RtspRequest::setMethod(Method m) { method = m; }
void RtspRequest::setUrl(const QString &u) { url = u; }
void RtspRequest::setCSeq(int s) { cseq = s; }

QByteArray RtspRequest::toByteArray() const
{
    QString methodStr = (method == OPTIONS) ? "OPTIONS" : "DESCRIBE";

    QString req;
    req += methodStr + " " + url + " RTSP/1.0\r\n";
    req += "CSeq: " + QString::number(cseq) + "\r\n";
    req += "User-Agent: QtRtspClient\r\n";

    if (method == DESCRIBE) {
        req += "Accept: application/sdp\r\n";
    }

    req += "\r\n";
    return req.toUtf8();
}