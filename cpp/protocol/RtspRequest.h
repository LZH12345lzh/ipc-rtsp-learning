#pragma once
#include <QString>

class RtspRequest
{
public:
    enum Method {
        OPTIONS,
        DESCRIBE
    };

    void setMethod(Method m);
    void setUrl(const QString &url);
    void setCSeq(int seq);

    QByteArray toByteArray() const;

private:
    Method method;
    QString url;
    int cseq = 1;
};