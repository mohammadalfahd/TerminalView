# TerminalView

A lightweight terminal-based financial charting application written in modern C++.

TerminalView renders financial candlestick charts directly inside the terminal using ASCII characters. The project is built from scratch without relying on GUI frameworks and focuses on chart rendering, viewport management, and technical analysis.

---

## Features

* 📈 ASCII candlestick chart rendering
* 🔍 Viewport-based chart navigation
* ↔️ Horizontal panning
* 🎯 Candle selection
* 📊 Dynamic price scaling
* 📉 20-Period Simple Moving Average (SMA)
* 📉 20-Period Exponential Moving Average (EMA)
* 📊 Volume Chart
* ℹ️ Status bar displaying candle information
* 🛠️ Modular and easy-to-extend codebase

---

## Controls

| Key   | Action                 |
| ----- | ---------------------- |
| **A** | Pan chart left         |
| **D** | Pan chart right        |
| **J** | Select previous candle |
| **L** | Select next candle     |
| **S** | Toggle SMA20           |
| **E** | Toggle EMA20           |
| **V** | Toggle Volume Chart    |
| **Q** | Exit the application   |

---

## Building

```bash
git clone https://github.com/mohammadalfahd/TerminalView.git

cd TerminalView

mkdir build
cd build

g++ -Iinclude *.cpp -o TerminalView

now TerminalView is ready for Launch:

./TerminalView

---

## Data Requirements

### CSV Format

The CSV file **must** contain columns in the following order:

```text
Date,Open,High,Low,Close,Adj Close,Volume
```

Changing the order of these columns may result in incorrect parsing or undefined behaviour.

---

### Valid Market Data

All manually entered or imported market data should represent realistic candlestick values.

Each candle should satisfy:

```text
High >= Open
High >= Close
Low <= Open
Low <= Close
High >= Low
```

Example of valid data:

```text
Open  = 102
High  = 108
Low   = 99
Close = 105
```

Example of invalid data:

```text
Open  = 102
High  = 101
Low   = 99
Close = 105
```

The second example is impossible because the closing price is higher than the recorded high.

Providing sensible market data ensures the chart and indicators behave correctly.

---

## Project Structure

```text
TerminalView/
│
├── include/
|   ├──indicators/
|   └── ...
├── src/
├── CSV_files/
├── README.md
└── ...
```

---

## Roadmap

* [x] Candlestick rendering
* [x] Dynamic price scaling
* [x] Viewport navigation
* [x] Candle selection
* [x] Simple Moving Average (SMA)
* [x] Exponential Moving Average (EMA)
* [ ] Relative Strength Index (RSI)
* [ ] MACD
* [ ] Bollinger Bands
* [x] Volume chart
* [ ] Multiple timeframes
* [ ] Additional technical indicators

---

## Screenshots

Screenshots and demonstrations will be added as the project evolves.

---

## About

TerminalView is a personal project created to explore software architecture, rendering techniques, and financial chart visualization using modern C++.

Instead of relying on graphical libraries, every candle, indicator, and chart element is rendered manually inside the terminal. The project serves as both a learning experience and a foundation for implementing additional charting features and technical indicators.

---

## License

This project is currently released for educational and personal use.
