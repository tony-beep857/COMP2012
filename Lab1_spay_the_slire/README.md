# COMP 2012 Lab 1: Spay the Slire

Welcome to **COMP 2012 Lab 1: Spay the Slire**, an Object-Oriented Programming (OOP) and Data Structures project written in C++. This project implements a turn-based, roguelike deck-building card game inspired by *Slay the Spire*.

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Intended Learning Outcomes](#-intended-learning-outcomes)
- [Game Mechanics & Rules](#-game-mechanics--rules)
  - [Game Flow](#game-flow)
  - [Card Types & Actions](#card-types--actions)
  - [Card Piles](#card-piles)
- [Project Structure](#-project-structure)
- [Tasks to Implement](#-tasks-to-implement)
  - [Part 1: Card Pile (`CardPile` Class)](#part-1-card-pile-cardpile-class)
  - [Part 2: Card Actions (`Card` Class)](#part-2-card-actions-card-class)
- [Building & Running](#-building--running)
  - [Compilation](#compilation)
  - [Running the Game](#running-the-game)
  - [Running Test Cases](#running-test-cases)
  - [Checking for Memory Leaks](#checking-for-memory-leaks)
- [Submission Guidelines & Constraints](#-submission-guidelines--constraints)

---

## 👁️ Overview

In **Spay the Slire**, you control the **Player** entity battling against an **Enemy** entity. Using a custom deck of cards, your goal is to reduce the enemy's health to $0$ before they defeat you.

### Key Differences from *Slay the Spire*:
1. **Single Enemy:** You fight only one enemy per game.
2. **Fixed Enemy Deck:** The Enemy plays cards from their own predefined deck sequentially.
3. **Persistent Block:** Block points accumulated during combat **do not** reset at the end of a turn.

---

## 🎯 Intended Learning Outcomes

By completing this lab, you will learn to:
1. Construct classes that manage **dynamically-allocated memory** using dynamic arrays.
2. Manage and manipulate **object pointers** safely.
3. Design dynamic game logic using Object-Oriented Programming (OOP) concepts.

---

## 🎮 Game Mechanics & Rules

### Game Flow

Each game consists of alternating turns starting with the Player:

1. **Player Turn Start:**
   - Energy resets to **3**.
   - Remaining cards in Hand are moved to the Discard Pile.
   - **5 cards** are drawn from the Draw Pile into the Hand.
2. **Player Action Phase:**
   - The player can play cards from their hand if they have sufficient energy.
   - Playing a card consumes its energy cost, moves the card to the Discard Pile, and executes its effect.
   - The player can play multiple cards per turn or end their turn early.
3. **Enemy Turn:**
   - The enemy plays **1 card** per turn sequentially from their deck without consuming energy.
   - After playing the last card in their deck, the enemy cycles back to the first card.
4. **Win/Loss Condition:**
   - The game ends immediately when either the Player's or Enemy's health reaches $0$ or lower.

---

### Card Types & Actions

| Type ID | Enum Type | Description |
| :---: | :--- | :--- |
| **0** | `DEAL_DAMAGE` | Deals damage equal to card potency to the target. Damage reduces Block first; remaining damage reduces Health. |
| **1** | `GAIN_BLOCK` | Adds block points equal to card potency to the caster. |
| **2** | `DRAW_CARDS` | Draws cards equal to card potency from the Draw Pile into Hand (Player only). |

---

### Card Piles

- **Draw Pile:** Cards available for drawing. If empty, cards from the Discard Pile are returned to the Draw Pile in their current discard order (no random shuffle required for this lab).
- **Hand:** Contains cards ready to be played (Maximum capacity: **10 cards**).
- **Discard Pile:** Stores played and unused cards.
- **Enemy Deck:** A single sequential deck used exclusively by the enemy.

---

## 📂 Project Structure

```text
lab1/
├── Card.hpp        # Header: Defines Card class properties and methods
├── CardPile.hpp    # Header: Defines CardPile class for managing arrays of Card pointers
├── Entity.hpp      # Header: Defines Player and Enemy stats (Health, Block, etc.)
├── GameState.hpp   # Header: Tracks active game state and turn cycles
├── lab_tasks.cpp   # SUBMISSION FILE: Task implementations go here
├── main.cpp        # Main entry point for user gameplay and standard testing
└── Makefile        # Build script for terminal compilation
```

---

## 🛠️ Tasks to Implement

All required logic must be written inside `lab_tasks.cpp`.

### Part 1: Card Pile (`CardPile` Class)

* **Task 1.1 — Constructor `CardPile(Card** cards, int card_count, int max_card_count)`**
  Allocates a dynamic array on the heap (using `new`) with maximum size `max_card_count`. Shallow copies pointers from the given `cards` array into this new array.

* **Task 1.2 — Destructor `~CardPile()`**
  Deallocates the dynamic array pointer memory. *(Note: Do not delete individual `Card` objects, as `CardPile` does not own them).*

* **Task 1.3 — `void add_card(Card* card)`**
  Appends a `Card` pointer to the end of the pile.

* **Task 1.4 — `Card* get_card(int index) const`**
  Returns the `Card` pointer at `index`. Returns `nullptr` if `index` is invalid.

* **Task 1.5 — `Card* remove_card(int index)`**
  Removes and returns the `Card` pointer at `index`. Shifts remaining element pointers to keep memory contiguous. Returns `nullptr` on invalid indices.

* **Task 1.6 — `void move_cards_to(CardPile& target)`**
  Appends all cards from the current pile to the `target` pile in order, resetting the current pile.

---

### Part 2: Card Actions (`Card` Class)

* **Task 2.1 — `void deal_damage(GameState* game_state)`**
  Deals damage equal to card potency to the target (Enemy during Player turn; Player during Enemy turn). Damage reduces target **Block** first. Any leftover damage reduces target **Health**.

* **Task 2.2 — `void gain_block(GameState* game_state)`**
  Adds block equal to card potency to the active caster.

*Helper functions `print_deal_damage_message()` and `print_gain_block_message()` are recommended when printing standard turn outputs.*

---

## 🚀 Building & Running

### Compilation

Build the project using `make`:

```bash
make
```

---

### Running the Game

Run the executable manually to play an interactive game:

```bash
# On Linux / macOS:
./lab1

# On Windows:
.\lab1.exe
```

---

### Running Test Cases

Test cases can be executed via input redirection:

```bash
# Linux / macOS
./lab1 < input_given/given_1.txt > output.txt

# Windows
.\lab1.exe < input_given\given_1.txt > output.txt
```

---

### Checking for Memory Leaks

To detect memory leaks or out-of-bound array errors, compile with AddressSanitizer (`ASan`) enabled on GCC/Clang:

```bash
g++ -std=c++17 -fsanitize=address,leak,undefined -g *.cpp -o lab1_debug
./lab1_debug < input_given/given_1.txt
```

---

## 🚫 Submission Guidelines & Constraints

- **Submission File:** Only `lab_tasks.cpp` must be submitted to **ZINC**.
- **No VLAs:** Variable Length Arrays (VLAs) are strictly forbidden. Use dynamic allocation via `new[]` and `delete[]`.
- **No Global/Static Variables:** Do not introduce global or static variables.
- **No `auto` / `decltype`:** Use explicit C++ standard type declarations.
- **No Extra Libraries:** Include only the headers supplied in the original skeleton code.