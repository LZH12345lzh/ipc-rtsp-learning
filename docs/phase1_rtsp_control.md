# RTSP Client Demo - Phase 1 (RTSP Control + SDP)

## 📌 1. 本阶段目标

本阶段仅实现 RTSP 协议的**控制层（Control Plane）通信**，用于完成媒体描述信息获取。

实现功能：

- TCP 连接 RTSP Server
- RTSP OPTIONS 请求
- RTSP DESCRIBE 请求
- RTSP Response 解析
- SDP 数据解析
- MediaSession 构建
- RTSP 状态机（到 Ready）

---

## 📡 2. RTSP Server

测试服务器：
rtsp://172.25.32.1:8554/live
（MediaMTX）

---

## 🔄 3. RTSP 流程

本阶段完整流程如下：
TCP Connect
↓
OPTIONS
↓
DESCRIBE
↓
SDP Response
↓
SDP Parse
↓
MediaSession
↓
Ready


---

## 🧠 4. RTSP 状态机

当前实现状态如下：
Idle
↓
Connecting
↓
Connected
↓
OptionsSent
↓
DescribeSent
↓
Ready
↓
Error


---

## 📤 5. RTSP Request（请求格式）

### OPTIONS
OPTIONS rtsp://host/path RTSP/1.0
CSeq: 1
User-Agent: QtRtspClient


---

### DESCRIBE

DESCRIBE rtsp://host/path RTSP/1.0
CSeq: 2
User-Agent: QtRtspClient
Accept: application/sdp

---

## 📥 6. RTSP Response（响应结构）

标准 RTSP Response：

RTSP/1.0 200 OK
CSeq: 2
Content-Type: application/sdp
Content-Length: xxx

v=0
o=-
s=No Name
c=IN IP4 0.0.0.0
t=0 0
m=video 0 RTP/AVP 96
a=control:trackID=0
a=rtpmap:96 H265/90000
a=fmtp:96 sprop-vps=...; sprop-sps=...; sprop-pps=...

---

## 🎯 7. SDP解析结果（MediaSession）

解析后的结构：
Track Count: 1

Track 0:

Media: video
Codec: H265
Payload Type: 96
Clock Rate: 90000
Control: trackID=0

---

## 🧩 8. 核心模块设计

### 8.1 RtspClient

职责：

- TCP连接管理
- RTSP状态机控制
- 请求发送
- 响应分发

---

### 8.2 RtspRequest

职责：

- 构造 RTSP 请求字符串
- 支持 OPTIONS / DESCRIBE

---

### 8.3 RtspResponse

职责：

- 解析 RTSP 响应
- 提取：
  - status code
  - headers
  - body (SDP)

---

### 8.4 SDPParser

职责：

- 解析 SDP 内容
- 提取媒体轨道信息

解析字段：

- m=video
- a=rtpmap
- a=control
- a=fmtp

---

### 8.5 MediaSession

用于保存媒体信息：


MediaTrack:

codec
payload type
clock rate
control

---

## 🔁 9. 当前系统架构


RtspClient
↓
RtspRequest
↓
QTcpSocket
↓
RtspResponse
↓
SDPParser
↓
MediaSession


---

## 📌 10. 关键设计原则

### ✔ 1. 状态机驱动

RTSP流程由 RtspClient 状态机控制，而不是 main.cpp 控制。

---

### ✔ 2. 分层解耦

| 模块 | 职责 |
|------|------|
| RtspClient | 网络 + 状态机 |
| RtspRequest | 请求构造 |
| RtspResponse | 响应解析 |
| SDPParser | SDP解析 |
| MediaSession | 数据结构 |

---

### ✔ 3. main.cpp 只负责观察结果

main 不控制协议流程，只接收结果：

sdpParsed(MediaSession)


---

## 🚨 11. 本阶段限制

Phase 1 不涉及任何媒体传输能力：

- 无 SETUP
- 无 RTP
- 无 UDP socket
- 无视频数据
- 无 H265解析

---

## 🚀 12. Phase 2 入口条件

进入下一阶段的前提：

- 已成功获取 SDP
- 已解析 trackID
- 已获得 payload type
- 已获得 control字段

---

## 📊 13. Phase 1 完成标志

当系统输出以下内容，说明 Phase 1 完成：
TCP Connected
OPTIONS OK
DESCRIBE OK

SDP READY
Codec: H265
Payload: 96
TrackID: 0


---

## 📅 Version

Phase: 1  
Status: Completed  
Date: 2026-06-28