# COMP 2012 Assignment 1: Online Shopping System

## Overview
This project implements an **Online Shopping System** in C++ for COMP 2012 (Object-Oriented Programming and Data Structures). The application models core e-commerce entities—including platforms, shops, customers, products, spend data tracking, and a shared rating system—using dynamic arrays, custom linked lists, and manual dynamic memory management.

---

## Intended Learning Outcomes
- Master C++ Object-Oriented Programming (OOP) concepts, class interactions, and raw pointer management.
- Implement dynamic array resizing ($2\times$ expansion factor) and dynamic memory allocation/deallocation (`new` / `delete`).
- Construct and manipulate custom singly/doubly linked lists without standard template libraries (STL).
- Handle shared resources across data structures (e.g., doubly-linked `Rating` nodes shared across `Customer` and `Shop` lists).

---

## File Structure

```text
pa1_student/
├── Customer.h / Customer.cpp           # Customer class & belongings array management
├── Shop.h / Shop.cpp                   # Shop inventory & rating list management
├── Platform.h / Platform.cpp           # Main system platform, VIP rules & shop hosting
├── Product.h / Product.cpp             # Product entity data
├── CustomerData.h / CustomerData.cpp   # Customer spending history (singly linked list node)
├── Rating.h / Rating.cpp               # Rating entity shared between customer and shop
├── main.cpp                            # System test runner (contains tests 1-44)
├── Makefile                            # Build instructions
└── tests/                              # Test input and expected output files
```

> **Note:** Only `Customer.cpp`, `Shop.cpp`, `Platform.cpp`, `Product.cpp`, `CustomerData.cpp`, and `Rating.cpp` are submitted for evaluation.

---

## Architecture & Relationships

1. **Ownership**:
   - `Platform` owns `Shop` pointers array and `CustomerData` linked list.
   - `Shop` owns unsold `Product` objects in dynamic array `inventory`.
   - `Customer` owns purchased `Product` objects in dynamic array `belongings`.
   - When a purchase occurs, the pointer to the `Product` transfers from `Shop::inventory` to `Customer::belongings` without destroying or re-instantiating the object.

2. **Shared Rating System**:
   - A single `Rating` object is referenced simultaneously by both a `Customer` rating list and a `Shop` rating list.
   - Decoupling logic ensures a `Rating` instance is safely deallocated from heap memory **only** when detached from both lists.

---

## Implementation Summary

### Task 1: Constructors & Destructors
- Set initial values, dynamic array initial capacities (default capacity = 2), auto-increment `rating_ID` counter, and setup null pointers.

### Task 2: Dynamic Arrays & 1D Linked Lists
- **Customer / Shop Dynamic Arrays**: Dynamic expansion ($2\times$) for `belongings`, `inventory`, and `shops`. Sorted insertion for shop inventory by `product_ID`.
- **Platform Linked Lists**: Ordered singly linked list for `CustomerData` nodes sorted ascending by `customer_ID`. VIP status check based on `VIP_threshold`.

### Task 3: Dual-Linked Rating System & Decoupling
- Rating validation (purchased items verification via `origin_ID`).
- Inserting/updating score $(0 \le \text{score} \le 100)$ and maintaining head placement.
- Two-stage explicit decoupling and memory cleanup upon deletion/destruction.

### Task 4: Business Logic & Operations
- **Order Handling (`Shop::handle_order`)**:
  - Calculates non-VIP vs. VIP discounted pricing and platform revenue cut.
  - Formula for VIP cut:
    $$\lfloor \text{Sum of Prices} \times \text{VIP Discount} \rfloor \times \frac{\text{Cut Rate}}{2}$$
  - Enforces transactional atomicity: fails safely without altering state if funds or inventory are insufficient.
- **Monthly Maintenance (`Platform::simulate_end_of_month`)**:
  - Deducts monthly subscription fees.
  - Removes shops failing to pay fee or scoring below threshold ($< 15$ avg score with $>3$ reviews).
  - Sorts active platform shops descending by average score.

---

## Compilation & Usage

### 1. Build
Compile the project using standard C++ compiler flags specified in the `Makefile`:
```bash
make
```

### 2. Run Test Cases
Execute test cases by redirecting standard input:
```bash
./shopping_system < tests/input_01.txt > actual_01.txt
diff -u tests/output_01.txt actual_01.txt
```
*If `diff` outputs nothing, your solution matched the expected test result.*

### 3. Memory Leak Check (Sanitizers)
To execute local sanitizer checks (Linux/macOS):
```bash
g++ -std=c++11 -Wall -Werror=vla -fsanitize=address,leak,undefined *.cpp -o shopping_system
./shopping_system < tests/input_01.txt
```

---

## Constraints & System Rules
- **No VLAs**: Variable Length Arrays are disabled via `-Werror=vla`. Use dynamic allocation (`new` / `delete[]`).
- **No External Libraries**: Standard containers (`std::vector`, `std::list`, etc.) are prohibited.
- **No Restricted Keywords**: Do not use `auto` or `decltype`.
- **No Extra Static/Global Variables**: Maintain strict encapsulation within class definitions.