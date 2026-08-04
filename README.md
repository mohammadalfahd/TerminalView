<div align="center">

# 📈 TerminalView

### A lightweight terminal-based financial charting engine built with modern C++17

Render historical and live financial charts directly inside the terminal using ASCII graphics.

**Real-time market data • Technical indicators • Multithreaded architecture • CMake**

<br>

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=c%2B%2B)
![Platform](https://img.shields.io/badge/Platform-Linux-success?style=for-the-badge&logo=linux)
![Build](https://img.shields.io/badge/Build-CMake-orange?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge)

</div>

---

## 📸 Demo

> **Coming soon:** Animated GIF demonstrating live market updates and interactive chart navigation.

<p align="center">
<img src="screenshots/CandlestickChart.jpg" width="900">
</p>

---

# ✨ Features

## 📊 Interactive charting

- ASCII candlestick charts
- Dynamic viewport rendering
- Automatic price scaling
- Interactive candle selection
- Horizontal panning
- Live status bar
- Responsive terminal layout

## 📈 Technical indicators

- SMA (20)
- EMA (20)
- RSI (14)
- MACD
- Volume histogram

## ⚡ Live market data

- Binance REST API integration
- Multiple timeframes
- Automatic live polling
- New candle detection
- Incremental indicator updates
- Thread-safe shared state

---

# 🏗 Architecture

TerminalView follows a modular architecture centered around a shared application state.

```text
                  Binance REST API
                         │
                         ▼
                 network_client
                         │
                         ▼
                  json_parser
                         │
                         ▼
                     app_state
              (mutex-protected state)
              ▲                  ▲
              │                  │
         polling_loop()     render_loop()
              │                  │
              ▼                  ▼
         Indicator Engine    Renderer + Viewport
              │                  │
              └──────────┬───────┘
                         ▼
                    ASCII Terminal
```

The application lifecycle is managed by the `application` class, while all shared runtime state is centralized in `app_state`, enabling thread-safe communication between rendering and network polling.

---

# 📁 Project structure

```text
TerminalView/
│
├── include/
│   ├── application/
│   │   ├── application.h
│   │   └── app_state.h
│   │
│   ├── chart/
│   │   ├── renderer.h
│   │   ├── viewport.h
│   │   └── grid.h
│   │
│   ├── data/
│   │   ├── candle.h
│   │   └── csv_loader.h
│   │
│   ├── indicators/
│   │   ├── indicators.h
│   │   ├── sma.h
│   │   ├── ema.h
│   │   ├── rsi.h
│   │   ├── macd.h
│   │   └── volume.h
│   │
│   ├── network/
│   │   ├── api_call.h
│   │   └── json_parser.h
│   │
│   └── terminal/
│       └── terminal.h
│
├── src/
│   ├── main.cpp
│   ├── application/
│   ├── chart/
│   ├── data/
│   ├── indicators/
│   └── network/
│
├── screenshots/
├── CSV_files/
├── build/
├── CMakeLists.txt
├── README.md
└── .gitignore
```

---

# ⚙️ Build

## Requirements

- C++17 compatible compiler
- CMake 3.16+
- libcurl
- pthreads

## Clone

```bash
git clone https://github.com/mohammadalfahd/TerminalView.git
cd TerminalView
```

## Configure

```bash
cmake -S . -B build
```

## Build

```bash
cmake --build build -j
```

## Run

```bash
./build/TerminalView
```

---

# 🧪 Debug builds

TerminalView includes optional sanitizer configurations.

## AddressSanitizer

```bash
cmake -S . -B build -DENABLE_ASAN=ON
cmake --build build
./build/TerminalView
```

## ThreadSanitizer

```bash
cmake -S . -B build -DENABLE_TSAN=ON
cmake --build build
./build/TerminalView
```

---

# 🎮 Controls

| Key | Action |
|-----|--------|
| **A** | Pan left |
| **D** | Pan right |
| **J** | Select previous candle |
| **L** | Select next candle |
| **S** | Toggle SMA |
| **E** | Toggle EMA |
| **V** | Toggle volume |
| **M** | Toggle MACD |
| **R** | Toggle RSI |
| **I** | Lower timeframe |
| **K** | Higher timeframe |
| **Q** | Quit |

---

# 🧠 Design highlights

## Incremental indicator engine

Indicators are updated incrementally whenever possible rather than recalculated from the entire dataset, reducing computational overhead during live polling.

## Thread-safe shared state

The renderer and network polling execute on separate threads while synchronizing through a mutex-protected `app_state` object.

## Modular architecture

Rendering, viewport management, networking, indicator calculations, and application control are implemented as independent components, making the project easier to maintain and extend.

## ASCII rendering engine

Every candlestick, axis, indicator, label, and chart element is rendered manually using terminal graphics without relying on GUI frameworks.

---

# 📊 Data sources

## CSV import

Expected format:

```text
Date,Open,High,Low,Close,Adj Close,Volume
```

## Live market data

TerminalView fetches real-time OHLCV data from the Binance REST API and automatically distinguishes between:

- updates to the currently forming candle
- newly completed candles

---

# 📷 Screenshots

| Main chart | SMA & EMA |
|------------|-----------|
| ![](screenshots/CandlestickChart.jpg) | ![](screenshots/smaema.jpg) |

| Volume | RSI |
|--------|-----|
| ![](screenshots/volume.jpg) | ![](screenshots/rsi.jpg) |

| MACD |
|------|
| ![](screenshots/MACD.jpg) |

---

# 🛣 Roadmap

- [x] ASCII candlestick rendering
- [x] Dynamic viewport
- [x] Interactive navigation
- [x] Live Binance data
- [x] Multithreaded rendering
- [x] SMA
- [x] EMA
- [x] RSI
- [x] MACD
- [x] Volume histogram
- [x] Multiple timeframes
- [x] Application architecture refactor
- [ ] WebSocket streaming
- [ ] Bollinger Bands
- [ ] Configuration system
- [ ] Performance profiling
- [ ] Cross-platform terminal support

---

# 📚 What I learned

This project provided hands-on experience with:

- Modern C++17
- Object-oriented design
- Software architecture
- Multithreading
- Mutex synchronization
- REST API integration
- JSON parsing
- Incremental algorithms
- Terminal rendering
- CMake
- Debugging concurrent systems

---

# 👨‍💻 About

TerminalView is a personal systems programming project focused on building a lightweight financial charting engine entirely inside the terminal.

The project emphasizes clean software architecture, modular design, efficient algorithms, and real-time market visualization using nothing more than ASCII graphics and modern C++.

---

# ⭐ Support

If you find this project interesting, consider giving the repository a **star**. It helps others discover the project and motivates future development.

---

# 📜 License

Released for educational and personal use.