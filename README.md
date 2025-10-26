# 🚦 Adaptive Traffic Light Simulation (C + Raylib)

![C](https://img.shields.io/badge/Language-C-blue)
![Library](https://img.shields.io/badge/Graphics-Raylib-ff69b4)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

A **real-time adaptive traffic light simulator** built in **C using Raylib**.  
This project demonstrates how traffic signals can adjust their timing dynamically based on the **number of vehicles** in each direction — similar to modern smart traffic systems.

---

## 🎮 Simulation Overview

This simulation models a **4-way intersection** with:
- North, South, East, and West lanes  
- Each direction has its own **green** and **yellow** phases  
- The **green light duration automatically adapts** to the number of waiting vehicles  
- Cars **arrive randomly** and **depart realistically** during the green phase  
- Everything is visually displayed using **Raylib graphics** (animated lights & vehicles)

---

## ✨ Features

- 🟢 **Adaptive green time:** More cars = longer green duration  
- 🚗 **Dynamic vehicle queue:** Random car generation per lane  
- 🟡 **Yellow phase** for safe transitions  
- 🔁 **Circular queue system** to handle all four signals  
- 💡 **Real-time graphical interface** using Raylib  
- 📈 **Displays active direction, time left, vehicle count, and flow info**

---

## 🧠 Concepts Demonstrated

- Data Structures: **Circular Queue**  
- Real-Time Simulation  
- Raylib Graphics Programming  
- Dynamic Timing Logic  
- Modular & Structured C Programming

---

## 📁 Project Structure

├── main.c # Complete source code
├── raylib-5.5_win64_mingw-w64/
│ └── include/raylib.h # Raylib header file
└── README.md # Project documentation


---

## ⚙️ Installation & Setup (Windows)

### 1. Clone the repository
```bash
git clone https://github.com/<your-username>/Adaptive-Traffic-Light-Simulation.git
cd Adaptive-Traffic-Light-Simulation


---

## ⚙️ Installation & Setup (Windows)

### 1. Clone the repository
```bash
git clone https://github.com/<your-username>/Adaptive-Traffic-Light-Simulation.git
cd Adaptive-Traffic-Light-Simulation

2. Install Raylib (Windows)

If you haven't installed Raylib yet:

Download from: https://www.raylib.com/

Extract it (already included in this project under raylib-5.5_win64_mingw-w64)

3. Compile the code

Using MinGW (GCC):

gcc main.c -o traffic_simulation.exe -I raylib-5.5_win64_mingw-w64/include -L raylib-5.5_win64_mingw-w64/lib -lraylib -lopengl32 -lgdi32 -lwinmm

4. Run the simulation
./traffic_simulation.exe

🖥️ Controls & Usage
Action	Description
Runs automatically	The simulation cycles through all signals dynamically
Close window	Press ESC or click close (Raylib default)

No keyboard or mouse input required — everything is automated.

🔍 How It Works

Each signal (North, South, East, West) is stored in a Circular Queue.

Vehicles are added randomly over time.

The active direction’s green duration = base + (vehicleCount × 0.5s)

Cars leave one by one every 0.5 seconds during green.

After green → yellow → next signal (queue rotation).

The interface updates in real-time showing:

Current active direction

Vehicles remaining

Light color (Green/Yellow)

Time left for current phase

Flow direction (e.g. “N → S, N → W”)

🧩 Technical Details
Component	Description
Language	C (GCC / MinGW)
Graphics Library	Raylib 5.5
Core Data Structure	Circular Queue
Algorithm Type	Adaptive / Real-time control
Randomness	Vehicle arrival rate
