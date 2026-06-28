#pragma once
#include <QString>
#include <QMap>

class RtspResponse
{
public:
    bool parse(const QByteArray &data);

    int statusCode() const { return m_statusCode; }
    QString body() const { return m_body; }

    QString header(const QString &key) const;

private:
    int m_statusCode = 0;
    QMap<QString, QString> m_headers;
    QString m_body;
};