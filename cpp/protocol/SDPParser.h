#pragma once
#include <QString>
#include "MediaSession.h"

class SDPParser
{
public:
    static MediaSession parse(const QString &sdp);
};