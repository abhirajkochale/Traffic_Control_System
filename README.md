# 🚦 Adaptive Traffic Control System Simulation

A **C-based simulation** of an **Adaptive Traffic Light Control System** using the **Raylib graphics library**.  
This project models a four-way intersection where traffic lights adaptively change based on vehicle density and timing logic — simulating a **smart traffic management system** in real-time.

---

## 📜 Table of Contents
- [Project Overview](#-project-overview)
- [Objectives](#-objectives)
- [Features](#-features)
- [System Requirements](#-system-requirements)
- [Installation & Setup](#️-installation--setup)
- [How to Compile and Run](#️-how-to-compile-and-run)
- [Project Structure](#-project-structure)
- [Code Explanation](#-code-explanation)
- [Demonstration](#-demonstration)
- [Screenshots](#-screenshots)
- [Future Improvements](#-future-improvements)
- [Contributors](#-contributors)
- [License](#-license)
- [Acknowledgments](#-acknowledgments)

---

## 🚘 Project Overview

The **Traffic Control System Simulation** aims to mimic a **real-world adaptive traffic signal** at a four-way intersection.  
Each direction (North, South, East, West) has its own signal, which alternates in a **circular queue sequence**. The system dynamically adjusts the **green light duration** depending on the **vehicle count** for each direction.

When a light turns green:
- Vehicles begin to move out one by one.
- The green light duration increases if more vehicles are waiting.
- When all vehicles have passed or time runs out, it switches to yellow, then moves to the next direction.

This demonstrates the concept of **intelligent traffic management systems** that optimize flow and reduce waiting time.

---

## 🎯 Objectives

- Simulate real-world **adaptive traffic light behavior**.
- Implement **Circular Queue** data structure in C.
- Visualize vehicle movement and signal changes using **Raylib**.
- Demonstrate **time-based** and **density-based** traffic control.
- Provide an **educational and interactive** traffic flow simulation.

---

## ✨ Features

✅ Real-time simulation of four-direction traffic.  
✅ Dynamic light control based on vehicle count.  
✅ Circular queue system to manage signal rotation.  
✅ Vehicle generation, movement, and departure logic.  
✅ Raylib-based 2D graphics visualization.  
✅ Live on-screen data display:
- Active direction  
- Current state (GREEN / YELLOW)  
- Vehicles remaining  
- Duration and time left  
✅ Adaptive green timing: more vehicles = longer green time.

---

## 💻 System Requirements

| Component | Minimum Requirement |
|------------|----------------------|
| **OS** | Windows 10 / Linux / macOS |
| **Compiler** | GCC / MinGW-w64 |
| **Library** | [Raylib 5.5](https://www.raylib.com/) |
| **RAM** | 2 GB or more |
| **Graphics** | Basic GPU support (OpenGL 3.3+) |

---

## ⚙️ Installation & Setup

### 1️⃣ Clone the Repository
```bash
git clone https://github.com/your-username/Traffic_Control_System.git
cd Traffic_Control_System
## 2️⃣ Install Raylib

Download **Raylib 5.5** for your operating system from [raylib.com](https://www.raylib.com/).

Extract it and set the **include** and **lib** paths properly in your compiler.

---

## 🏗️ How to Compile and Run

### 🪟 On Windows (MinGW)
```bash
gcc main.c -o traffic_sim -I"raylib-5.5_win64_mingw-w64/include" -L"raylib-5.5_win64_mingw-w64/lib" -lraylib -lopengl32 -lgdi32 -lwinmm
.\traffic_sim
Traffic_Control_System/
│
├── main.c                          # Main simulation source code
├── raylib-5.5_win64_mingw-w64/     # Raylib library (headers & libs)
├── README.md                       # Documentation
├── /screenshots                    # Optional: simulation images
└── LICENSE                         # Open source license
