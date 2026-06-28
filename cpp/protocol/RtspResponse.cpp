#include "RtspResponse.h"

bool RtspResponse::parse(const QByteArray &data)
{
    QList<QByteArray> parts = data.split('\n');

    bool headerDone = false;

    for (auto &line : parts)
    {
        QString l = QString::fromUtf8(line).trimmed();

        if (l.startsWith("RTSP/1.0"))
        {
            QStringList tokens = l.split(' ');
            if (tokens.size() >= 2)
                m_statusCode = tokens[1].toInt();
        }
        else if (!headerDone && l.contains(":"))
        {
            auto kv = l.split(":");
            if (kv.size() >= 2)
                m_headers[kv[0].trimmed()] = kv[1].trimmed();
        }
        else
        {
            headerDone = true;
            m_body += l + "\n";
        }
    }

    return m_statusCode == 200;
}

QString RtspResponse::header(const QString &key) const
{
    return m_headers.value(key);
}