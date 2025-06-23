# LCD-Keypad Guess Number Game (8051 Project)

A simple number guessing game implemented on the 8051 microcontroller using a matrix keypad for input and an LCD1602 display for output.

## 🔧 Project Overview

This project implements a classic **"Guess the 4-digit number"** game. The system randomly generates a 4-digit number. The player guesses via a **4x4 matrix keypad**, and the system gives feedback in the form of `xAyB`:
- **A**: Number of digits correct in both **value and position**
- **B**: Number of digits correct in **value but wrong position**

If the player guesses correctly within 7 tries, they win. Otherwise, the correct answer is revealed.

## 🎮 Demo
<img src="https://github.com/user-attachments/assets/e5b08322-314c-4214-b4de-5d551977902e" width="640" height="360" alt="GuessNum_Demo">

> Entering digits, confirming guesses, viewing history, and game reset in action.

## 💡 Features

- 4-digit number generation (with possible duplicate digits)
- LCD1602 display for prompts and feedback
- Matrix keypad for input
- Up to 7 attempts per game
- Scrollable guess history
- Game reset functionality

## 🛠️ Hardware Requirements

- **Microcontroller:** STC89C52RC (8051-compatible MCU)  
- **Compiler Target:** AT89C52 (compatible instruction set and architecture)  
- **Display:** LCD1602 (8-bit interface)  
- **Input Device:** 4x4 Matrix Keypad  

### Note

The STC89C52RC is an 8051-core microcontroller highly compatible with the AT89C52 in terms of instruction set and hardware architecture. Therefore, using the AT89C52 compiler target and header files is a common and practical approach. This project’s actual hardware is the STC89C52RC, clarified here to avoid any confusion.

## 📁 Project Structure
```
GuessNumber-LCD-Keypad/
├── include/ # Header files
│ ├── Delay.h
│ ├── LCD1602.h
│ └── MatrixKey.h
├── src/ # Source code
│ ├── Delay.c
│ ├── LCD1602.c
│ ├── MatrixKey.c
│ └── main.c # Main game logic
├── project.uvproj # Keil uVision project file
├── project.uvopt
├── .gitignore
└── README.md
```

## 🚀 Getting Started

1. Open `project.uvproj` using **Keil uVision IDE**
2. Connect your development board and build the project
3. Flash the compiled HEX to your 8051 microcontroller
4. Start playing by entering guesses using the keypad!

## 🔢 Keypad Layout Mapping

| Button | Function      |
|--------|---------------|
| 1-10   | Input digits  |
| 13     | Confirm guess |
| 12     | Scroll ↑ history |
| 16     | Scroll ↓ history |
| 14     | Reset game    |

## ✍️ Author

**Andy Shao**  
GitHub: [@AndyCodee](https://github.com/AndyCodee)

---



