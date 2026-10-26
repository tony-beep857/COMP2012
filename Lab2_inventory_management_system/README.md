# Inventory Management System (Doubly-Linked List)

An Object-Oriented Programming (OOP) and Data Structures project written in C++. This project implements an **Inventory System** powered by a doubly-linked list of `Slot` nodes bounded by dummy `HEAD` and `TAIL` nodes, supporting deep-copying, capacity bounds, dynamic node linking, and memory cleanup.

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Key Features](#-key-features)
- [Data Structures & Architecture](#-data-structures--architecture)
  - [The `Slot` Class](#the-slot-class)
  - [The `Inventory` Class](#the-inventory-class)
- [Member Functions & Requirements](#-member-functions--requirements)
- [Build & Execution](#-build--execution)
- [Test Suite Summary](#-test-suite-summary)

---

## 🛠 Overview

The goal of this project is to manage items stored in inventory slots using dynamic memory and linked list pointers. The structure ensures $O(1)$ insertion and deletion once target nodes are reached, while strictly enforcing:
- Maximum item capacity constraints per `Slot`.
- Maximum slot capacity constraints for the `Inventory`.
- Deep copies of dynamic strings (item names) and inventory instances.
- Double-link consistency (`prev` and `next` pointers) at all times.

---

## ✨ Key Features

1. **Doubly-Linked List Architecture**: Every `Slot` maintains explicit pointers to both its predecessor (`prev`) and successor (`next`).
2. **Dummy Sentinel Nodes**: The inventory uses dummy `HEAD` and `TAIL` nodes to simplify insertion and deletion edge cases (e.g., operating on empty lists or list boundaries).
3. **Deep Copying**:
   - `Slot` names are stored via dynamic C-strings (`char*`) and must be deep-copied.
   - `Inventory` copy constructor creates independent copies of all slots and dynamic names.
4. **Capacity Management**: Handles overflow when adding items and automatically removes `Slot` nodes when their item count reaches zero.

---

## 🏗 Data Structures & Architecture

### The `Slot` Class

Represents an individual item stack within the inventory.

* **Attributes**:
  * `char* m_name`: Dynamic array storing the item name.
  * `int m_count`: Current quantity of items in the slot.
  * `int m_max_items`: Maximum capacity of items this slot can hold.
  * `Slot* m_prev`: Pointer to the preceding slot node.
  * `Slot* m_next`: Pointer to the succeeding slot node.

* **Constructors & Destructor**:
  * `Slot(const char* name, int max_items, Slot* prev)`: Creates a new slot with `count = 0`, deep-copies `name`, and automatically inserts itself directly after `prev`.
  * `Slot(const Slot& other, Slot* prev)`: Copy constructor that creates a new slot from `other` and inserts it after `prev`.
  * `~Slot()`: Deallocates `m_name` and safely unlinks itself from the doubly-linked list by updating `prev->next` and `next->prev`.

### The `Inventory` Class

Manages the collection of `Slot` nodes.

* **Attributes**:
  * `Slot* m_head`: Dummy sentinel node representing the start of the list (`HEAD_NAME`).
  * `Slot* m_tail`: Dummy sentinel node representing the end of the list (`TAIL_NAME`).
  * `int m_capacity`: Maximum number of slots allowed in the inventory.
  * `int m_count`: Current number of active slots (excluding sentinel nodes).

---

## 📑 Member Functions & Requirements

### `Slot` Operations
* `get_name() const`: Returns the item name pointer.
* `get_count() const`: Returns current item count.
* `get_max_items() const`: Returns maximum allowed items in this slot.
* `get_prev() const` / `get_next() const`: Accessor functions for linked pointers.
* `fill(int amount)`: Adds items up to `m_max_items`.

### `Inventory` Operations
* `Inventory(int capacity)`: Constructs an empty inventory with dummy `head` and `tail` nodes linked together.
* `Inventory(const Inventory& other)`: Deep-copies all slots and internal data from `other`.
* `~Inventory()`: Deallocates all slots including dummy nodes to prevent memory leaks.
* `add(const char* name, int max_items, int count)`: 
  * Adds a new slot at the end of the inventory if capacity permits.
  * Fills the new slot up to `max_items`.
  * Returns leftover items that could not fit, or `-1` if the inventory is already full (`count == capacity`).
* `take(int index, int count)`:
  * Takes `count` items from the 0-indexed slot.
  * If the slot count reaches 0, the slot is destroyed/deleted and automatically unlinked.
  * Returns leftover items requested but unavailable, or `-1` if index is out of bounds.

---

## 💻 Build & Execution

### Prerequisites
* C++ compiler (`g++` or `clang++`) supporting C++11 or later.

### Compilation
Compile the program using `g++`:

```bash
g++ -std=c++11 -Wall main.cpp inventory.cpp -o inventory_app
```

### Running Tests

Run all test cases sequentially:
```bash
./inventory_app
```

Run a specific test case (e.g., Test 4):
```bash
./inventory_app 4
```

---

## 🧪 Test Suite Summary

The test cases in `main.cpp` cover the following functionality:

| Test Case | Focus Area | Description |
| :--- | :--- | :--- |
| **Test 1** | `Slot` Constructors & Insertion | Verifies deep copy of names, default values, and double-linking when inserting at head, middle, and tail. |
| **Test 2** | `Slot` Destructor & Unlinking | Tests node deletion from middle, start, and end positions, verifying `prev` and `next` pointers stay consistent. |
| **Test 3** | `Inventory` Life Cycle | Verifies initial dummy node setup (`HEAD_NAME` / `TAIL_NAME`) and allocation/destruction of empty inventories. |
| **Test 4** | `add()` and `take()` Mechanics | Tests capacity limits, overflow management, index validation, middle slot removal on depletion, and list consistency. |
| **Test 5** | Deep Copying `Inventory` | Verifies full independent deep copies of inventories, ensuring memory modifications/destructions do not affect the original. |
| **Test 6** | End-to-End Scenario | Simulates a game state backup before a boss fight, item consumption, inventory wipeout, and state restoration from backup. |