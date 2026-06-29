MediaMTX
    │
    │ RTSP
    ▼
Qt RtspClient
    │
    ├── OPTIONS
    ├── DESCRIBE
    ├── SETUP
    ▼
RTSP Session Ready
    │
    │ RTP (UDP)
    ▼
RtpReceiver
    │
    ├── RTP Header Parser
    ├── Sequence Check
    ├── Timestamp
    ▼
H265 Parser (Phase3)
    │
    ▼
Frame Assembler

✅ Phase2.1
🎯 目标：RTSP控制层建立
TCP RTSP连接
OPTIONS 请求
DESCRIBE 请求
SDP解析
Track解析（MediaSession）
SETUP 请求
RTP端口协商
RTSP Session建立