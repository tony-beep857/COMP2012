# COMP 2012 Lab 4: Sokoban Game

Welcome to **COMP 2012 Lab 4: Sokoban Game**, an Object-Oriented Programming (OOP) project in C++. In this lab, you will implement a modified version of the classic Sokoban puzzle game featuring multi-box pushing mechanics, doors, buttons, and door jamming mechanics using C++ class inheritance and dynamic object interaction.

---

## 📋 Table of Contents
- [Objectives & Learning Outcomes](#-objectives--learning-outcomes)
- [Game Rules & Features](#-game-rules--features)
- [Game Objects & Rendering](#-game-objects--rendering)
- [Key Mechanics](#-key-mechanics)
  - [Multi-Box Pushing](#1-multi-box-pushing)
  - [Door Jamming](#2-door-jamming)
- [Project Architecture](#-project-architecture)
- [Tasks & Implementation Guidelines](#-tasks--implementation-guidelines)
  - [Part 1: Derived Constructors](#part-1-derived-constructors)
  - [Part 2: Object Behaviors](#part-2-object-behaviors)
- [Compilation & Testing](#-compilation--testing)
- [Implementation Constraints](#-implementation-constraints)

---

## 🎯 Objectives & Learning Outcomes

By completing this lab, you will be able to:
1. **Use Inheritance** to share common attributes and behaviors between derived game entity classes.
2. **Implement Specialized Derived Functionality** to modify behaviors, state properties, collision policies, and rendering sprites.
3. **Write Recursive Movement Logic** allowing players to push multi-box chains across the grid.

---

## 🎮 Game Rules & Features

Unlike traditional Sokoban:
- The **Win Condition** requires the Player (`P`) to reach the **Flag** (`!`).
- The player can **push multiple boxes** simultaneously in a line if space permits.
- **Buttons** control **Doors**. Pressing a button opens its paired door; stepping off attempts to close it.
- **Door Jamming**: Doors cannot close if occupied by a player or box.

---

## 👾 Game Objects & Rendering

All game entities derive from the abstract base class `GameObject`. Objects render using 3-character-wide string representations, allowing layered visuals for overlapping grid cells (e.g., `_P_` for a Player standing on a Button).

| Class | Base Sprite | Render Priority | Is Collidable? | Is Movable? | Description |
| :--- | :---: | :---: | :---: | :---: | :--- |
| `Player` | ` P ` | 6 | Yes | Yes | Controlled by user input (`w`, `a`, `s`, `d`). |
| `Wall` | `###` | 5 | Yes | No | Unmovable obstacle. |
| `Box` | ` X ` | 4 | Yes | Yes | Pushed by player or other boxes. |
| `Door` (Closed) | `[#]` | 3 | Yes | No | Blocks passage when closed. |
| `Door` (Open) | `[ ]` | 3 | No | No | Allows passage when open. |
| `Button` (Unpressed)| `---` | 2 | No | No | Controls linked door state. |
| `Button` (Pressed) | `___` | 2 | No | No | Activated when stepped on by a collidable object. |
| `Flag` | ` ! ` | 1 | No | No | Victory destination tile. |

---

## ⚙️ Key Mechanics

### 1. Multi-Box Pushing
When an entity attempts to move into a cell occupied by another collidable object:
- It recursively queries if the target object can also move in that direction (`try_move`).
- If the chain of objects ends at an empty cell or uncollidable object, the entire line moves forward.
- If blocked by a wall or closed door, none of the objects move.

### 2. Door Jamming
- A door attempts to close (`close_door`) when its button is released.
- If another collidable object (player or box) shares the same coordinate `(x, y)` as the door, the door becomes **jammed** and stays open until cleared.

---

## 📁 Project Architecture

```
lab4/
├── GameObject.hpp   # Header for base abstract class GameObject
├── GameBoard.hpp    # Class managing object storage and grid queries
├── objects.hpp      # Header declarations for Player, Wall, Box, Door, Button, Flag
├── lab_tasks.cpp    # [ONLY FILE TO SUBMIT] Implementation of lab functions
├── main.cpp         # Main game loop, input processing, and test runner
└── Makefile         # Build configuration script
```

---

## 🛠️ Tasks & Implementation Guidelines

All implementations must be placed inside **`lab_tasks.cpp`**.

### Part 1: Derived Constructors

Initialize derived class members and invoke the `GameObject` parent constructor.

- **Task 1.1: `Player::Player(int x, int y)`**
  - Sets sprite to `" P "`, rendering priority to `6`.
- **Task 1.2: `Wall::Wall(int x, int y)`**
  - Sets sprite to `"###"`, rendering priority to `5`.
- **Task 1.3: `Box::Box(int x, int y)`**
  - Sets sprite to `" X "`, rendering priority to `4`.
- **Task 1.4: `Door::Door(int x, int y)`**
  - Default state: closed (`[#]`), rendering priority `3`.
- **Task 1.5: `Button::Button(int x, int y, Door* door)`**
  - Default sprite `"---"`, rendering priority `2`, links target `door`.
- **Task 1.6: `Flag::Flag(int x, int y)`**
  - Sets sprite to `" ! "`, rendering priority to `1`.

---

### Part 2: Object Behaviors

- **Task 2.1: `GameObject* GameBoard::find_collidable_object(int x, int y)`**
  - Iterates through `board` elements to return the first collidable object at `(x, y)`. Returns `nullptr` if empty.

- **Task 2.2: `bool GameObject::try_move(GameBoard& board, Direction direction)`**
  - Checks target cell coordinates.
  - If a collidable object is present, recursively calls `try_move` on that object.
  - Updates entity `(x, y)` coordinates upon successful move and returns `true`.

- **Task 2.3: `void Door::open_door()`**
  - Changes sprite to `"[ ]"` and sets `is_collidable = false`.

- **Task 2.4: `void Door::close_door(const GameBoard& board)`**
  - Checks if another collidable object occupies the door's cell `(x, y)`.
  - If free: sets sprite to `"[#]"` and `is_collidable = true`.
  - If occupied: door remains jammed/open.

- **Task 2.5: `void Button::update_button(const GameBoard& board)`**
  - Queries `(x, y)` position for any collidable object.
  - If occupied: updates sprite to `"___"` and calls `open_door()`.
  - If empty: updates sprite to `"---"` and calls `close_door()`.

---

## 💻 Compilation & Testing

### Build Command
Compile the project using the provided Makefile:
```bash
make
```

### Running Test Cases
Run built-in test inputs:

- **Linux / Mac:**
  ```bash
  ./lab4 < tests/input/given_01.txt > output.txt
  ```
- **Windows:**
  ```cmd
  lab4.exe < tests\input\given_06.txt > output.txt
  ```

---

## 🚫 Implementation Constraints

1. **Submit File:** Only submit `lab_tasks.cpp` to ZINC.
2. **No Additional Libraries:** Do not `#include` unauthorized headers.
3. **No Dynamic Arrays / VLAs:** Avoid Variable Length Arrays. Use dynamic memory allocations (`new` / `delete`) if required.
4. **Forbidden Keywords:** Do **NOT** use `auto` or `decltype`.
5. **No Static/Global Variables:** Do not declare `static` or global storage variables.
6. **Honor Code:** Plagiarism detection software is actively enforced.