# 🐍 Snake Game — C++ OOP

A console-based Snake Game built in **C++** demonstrating core programming concepts including Object-Oriented Programming and Linked List data structures.

## 🚀 Features

- **Linked List** for dynamic snake body management
- **OOP Design** — `Snake`, `Food`, `Game` classes with clear responsibilities
- **Collision Detection** — wall & self-collision
- **Progressive Difficulty** — speed increases every 50 points
- **Score & High Score** tracking
- **Restart system** without restarting the program
- **WASD + Arrow Key** controls

## 🧱 Tech Stack

- Language: **C++**
- Platform: **Windows (Console)**
- Libraries: `<windows.h>`, `<conio.h>` (standard Windows headers)

## 📂 Project Structure

```
snake_game/
└── snake.cpp      # Full game source code
```

## 🔧 How to Compile & Run

### Using g++ (MinGW):
```bash
g++ snake.cpp -o snake.exe
./snake.exe
```

### Using Dev-C++ / Code::Blocks:
Open `snake.cpp` and hit **Run**.

## 🎮 Controls

| Key       | Action     |
|-----------|------------|
| `W` / `↑` | Move Up    |
| `S` / `↓` | Move Down  |
| `A` / `←` | Move Left  |
| `D` / `→` | Move Right |
| `Q`       | Quit       |
| `R`       | Restart    |

## 📐 OOP Design

| Class   | Responsibility                              |
|---------|---------------------------------------------|
| `Node`  | Linked list node storing body segment (x,y) |
| `Snake` | Manages snake movement, growth, collision   |
| `Food`  | Spawns and draws food randomly              |
| `Game`  | Main game loop, input, HUD, score, levels   |

## 📊 Concepts Demonstrated

- ✅ Linked List (dynamic snake body)
- ✅ OOP (classes, encapsulation, destructors)
- ✅ Collision Detection algorithm
- ✅ Game loop architecture
- ✅ Memory management (`new` / `delete`)
- ✅ Enums for clean state management

---
*Built as a C++ project demonstrating data structures and OOP principles.*
