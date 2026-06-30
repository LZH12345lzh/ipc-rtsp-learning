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

## Phase 2.1
🎯 目标：RTSP控制层建立
TCP RTSP连接
OPTIONS 请求
DESCRIBE 请求
SDP解析
Track解析（MediaSession）
SETUP 请求
RTP端口协商
RTSP Session建立

## Phase 2.2 — RTP over TCP 数据通路打通
📌 本阶段目标
在完成 RTSP 控制通路（OPTIONS / DESCRIBE / SETUP / PLAY）的基础上，实现：

✔ RTP over TCP（Interleaved Mode）数据接收
✔ RTSP 与 RTP 解耦处理
✔ H265 RTP payload 获取

🧠 架构变化（关键升级）
Phase2.1：

QTcpSocket → RtspClient → 解析 RTSP Response

Phase2.2：

QTcpSocket
    ↓
RtspClient::onReadyRead（唯一入口）
    ↓
├── RTSP Response Parser
└── RTP Interleaved Parser
        ↓
     RtpReceiver
🔧 核心改动点
1. 统一 socket 读取入口

所有数据统一由：

RtspClient::onReadyRead()

处理，避免多个模块同时 readAll() 导致数据丢失。

2. RTP over TCP 解析

RTSP interleaved RTP format：

$ | channel | length(2 bytes) | RTP payload

解析逻辑：

判断 $ 开头
读取 channel
读取 length
提取 RTP payload
交给 RtpReceiver
3. RtpReceiver 职责收敛

RtpReceiver 不再持有 socket：

❌ 不再 attachSocket()
❌ 不再 readAll()
✔ 只负责 RTP payload 解析