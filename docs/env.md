# Project Environment

## 🧠 Build Environment

- OS: WSL (Ubuntu)
- Compiler: GCC (WSL toolchain)
- Build System: qmake 3.1
- Qt Version: 5.12.8

---

## 🌐 Runtime Environment

- RTSP Server: MediaMTX (Windows host)
- RTSP URL: rtsp://172.25.32.1:8554/live

---

## ⚠️ Important Notes

### 1. WSL networking behavior

- WSL can access Windows host via:
  - 172.25.32.1 (WSL gateway)
- localhost (127.0.0.1) may NOT reach Windows services

---

### 2. Qt version constraints

This project uses Qt 5.12.8:

- QTcpSocket signals use legacy overload handling
- QOverload is required for errorOccurred()
- C++17 features partially supported

---

### 3. Build system

qmake is used instead of CMake:

- $$files() used for source aggregation
- No automatic dependency scanning like CMake