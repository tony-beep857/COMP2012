# COMP 2012 Lab 5: Naive Solar System Simulator

Welcome to **COMP 2012 Lab 5: Naive Solar System Simulator**, an Object-Oriented Programming (OOP) project in C++. In this lab, you will implement a 2D solar system simulator featuring stars, planets, moons, and interstellar objects using dynamic binding, dynamic dispatch, and runtime polymorphism.

---

## 📋 Table of Contents

* [Objectives & Learning Outcomes](#-objectives--learning-outcomes)
* [Simulation & Physics Model](#-simulation--physics-model)
* [Class Hierarchy & Architecture](#-class-hierarchy--architecture)
* [Tasks & Implementation Details](#-tasks--implementation-details)
  * [Part 1: Interstellar Objects](#part-1-interstellar-objects)
  * [Part 2: Orbiters, Planets, & Moons](#part-2-orbiters-planets--moons)
* [Helper Class: Vector2](#-helper-class-vector2)
* [Compilation & Testing](#-compilation--testing)
* [Implementation Rules & Constraints](#-implementation-rules--constraints)

---

## 🎯 Objectives & Learning Outcomes

By completing this lab, you will learn to:

1. **Reuse Code Efficiently** in polymorphic class structures.
2. **Apply Dynamic Binding & Virtual Functions** to access derived behaviors through base class references or pointers.
3. **Model Complex Hierarchical Relationships** (such as Stars $\rightarrow$ Planets $\rightarrow$ Moons) using inheritance.

---

## 🌌 Simulation & Physics Model

To keep calculations straightforward, the simulator uses simplified physical assumptions:

* **Fixed Reference Frame:** The `Star` is stationary at its position.
* **Circular Orbits:** All orbiting bodies (`Orbiter` derivatives like `Planet` and `Moon`) move in uniform circular orbits.
* **Linear Trajectories:** `InterstellarObject` instances move in straight lines with constant velocity vectors.
* **Local Orbital Centers:** A planet orbits its Star, while a moon orbits its parent Planet (which itself orbits the Star).
* **Point-Mass Dynamics:** Collisions between celestial bodies are ignored.

---

## 🏗️ Class Hierarchy & Architecture

```
                       ┌─────────────────┐
                       │ CelestialObject │ (Abstract Base)
                       └────────┬────────┘
             ┌──────────────────┼──────────────────┐
             ▼                  ▼                  ▼
   ┌────────────────────┐   ┌─────────┐       ┌──────────┐
   │ InterstellarObject │   │ Orbiter │ (Abs) │   Star   │
   └────────────────────┘   └────┬────┘       └──────────┘
                             ┌───┴───┐
                             ▼       ▼
                        ┌────────┐ ┌──────┐
                        │ Planet │ │ Moon │
                        └────────┴────────┘
```

### File Structure Overview

| File Name | Required Submission? | Description |
| :--- | :---: | :--- |
| `CelestialObject.h` | No | Abstract base class header for all celestial bodies. |
| `Star.h` / `Star.cpp` | No | Stationary star class implementation (provided). |
| `InterstellarObject.h` / `InterstellarObject.cpp` | **Yes** | Linear moving object header and implementation. |
| `Orbiter.h` / `Orbiter.cpp` | **Yes** | Abstract base orbiter header and implementation. |
| `Planet.h` / `Planet.cpp` | **Yes** | Planet class header and implementation. |
| `Moon.h` / `Moon.cpp` | **Yes** | Moon class header and implementation. |
| `Vector2.h` | No | 2D vector utility class (provided). |
| `testcases.h` / `main.cpp` | No | Test runner and entry point. |

---

## 🛠️ Tasks & Implementation Details

You are required to implement functions inside **`InterstellarObject.cpp`**, **`Orbiter.cpp`**, **`Planet.cpp`**, and **`Moon.cpp`**.

### Part 1: Interstellar Objects

Implemented in `InterstellarObject.cpp`:

* **`Vector2 InterstellarObject::get_position() const`**
  Returns the current position of the interstellar object.

* **`void InterstellarObject::simulate(double years)`**
  Updates the object's age and computes its new position:
  $$\text{position}_{\text{new}} = \text{position}_{\text{current}} + (\text{years} \times \text{velocity})$$

---

### Part 2: Orbiters, Planets, & Moons

#### 1. `Orbiter` Class (`Orbiter.cpp`)

* **`Orbiter::Orbiter(const string &name, double age, const CelestialObject &orbitee, double orbital_radius, double angular_speed)`**
  Initializes member variables. Sets initial `angular_position = 0.0` radians.

* **`Vector2 Orbiter::get_position() const`**
  Computes global position using the orbitee's position, orbital radius $r$, and current angular position $\theta$:
  $$\mathbf{P}_{\text{orbiter}} = \mathbf{P}_{\text{orbitee}} + \begin{bmatrix} r \cos(\theta) \\ r \sin(\theta) \end{bmatrix}$$

* **`void Orbiter::simulate(double years)`**
  Updates age and updates angular position:
  $$\theta_{\text{new}} = \theta_{\text{current}} + (\text{angular\_speed} \times \text{years})$$

* **`double Orbiter::get_distance_from_star() const`**
  Calculates distance from the solar system's `Star` to this orbiter:
  $$\text{distance} = \| \mathbf{P}_{\text{orbiter}} - \mathbf{P}_{\text{star}} \|$$

#### 2. `Planet` Class (`Planet.cpp`)

* **`Planet::Planet(const string &name, double age, const Star &orbitee, double orbital_radius, double angular_speed)`**
  Passes arguments to `Orbiter` base constructor.

* **`const Star& Planet::get_star() const`**
  Returns a reference to the `Star` that this planet orbits.

#### 3. `Moon` Class (`Moon.cpp`)

* **`const Star& Moon::get_star() const`**
  Traverses the orbital hierarchy to return the underlying `Star` that the moon's parent `Planet` orbits.

---

## 📐 Helper Class: Vector2

The provided `Vector2` class includes useful member and static helper methods:

```cpp
double get_magnitude() const;                             // Returns magnitude of vector
static Vector2 add(const Vector2 &a, const Vector2 &b);      // Returns (a + b)
static Vector2 subtract(const Vector2 &a, const Vector2 &b); // Returns (a - b)
static Vector2 scale(const Vector2 &v, double factor);      // Returns (v * factor)
```

---

## 💻 Compilation & Testing

### Build Command

Compile using the Makefile provided:

```bash
make
```

### Running Test Cases

Run individual tests or all test cases:

* **Linux / macOS:**
  ```bash
  ./lab5 1    # Run test case 1
  ./lab5      # Run all test cases
  ```

* **Windows:**
  ```cmd
  lab5.exe 1  # Run test case 1
  lab5.exe    # Run all test cases
  ```

---

## 🚫 Implementation Rules & Constraints

1. **Submission Files:** Only submit `InterstellarObject.cpp`, `Moon.cpp`, `Orbiter.cpp`, and `Planet.cpp` to ZINC.
2. **No Extra Libraries:** Include only the headers provided in the skeleton code.
3. **No Dynamic Arrays / VLAs:** Variable Length Arrays are disabled via `-Werror=vla`. Use dynamic allocation (`new`/`delete`) if necessary.
4. **Forbidden Keywords:** Do **NOT** use `auto` or `decltype`.
5. **No Static or Global Variables:** Do not declare global variables or static storage within functions/files.
6. **Honor Code:** All code must be your own original work. Plagiarism detection tools are routinely run.