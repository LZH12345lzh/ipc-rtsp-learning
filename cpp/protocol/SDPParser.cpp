#include "SDPParser.h"

MediaSession SDPParser::parse(const QString &sdp)
{
    MediaSession session;
    MediaTrack track;

    QStringList lines = sdp.split('\n');

    for (const QString &l : lines)
    {
        QString line = l.trimmed();

        if (line.startsWith("m=video"))
        {
            track.media = "video";
        }
        else if (line.startsWith("a=rtpmap:"))
        {
            // a=rtpmap:96 H265/90000
            QString tmp = line.section(':',1);
            QString pt = tmp.section(' ',0,0);
            QString codec = tmp.section(' ',1,1);

            track.payloadType = pt.toInt();
            track.codec = codec.section('/',0,0);
            track.clockRate = codec.section('/',1,1).toInt();
        }
        else if (line.startsWith("a=control:"))
        {
            track.control = line.section(':',1);
            // QString control = line.section(':',1);
            // if (!control.startsWith("streamid")) {
            //     track.control = "streamid=" + control.section('=',1);
            // } else {
            //     track.control = control;
            // }
        } 
    }

    session.tracks.append(track);
    return session;
}