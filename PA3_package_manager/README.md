# COMP 2012 Assignment 3: Package Manager

A lightweight C++17 software package manager implementation inspired by Linux tools such as `pacman`, `apt`, and `dnf`. This project demonstrates advanced Object-Oriented Programming (OOP), modern C++ Standard Template Library (STL) features, custom template-based data structures (Hash Trie), dependency resolution algorithms, semantic versioning, and non-destructive transaction safety.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Objectives & Learning Outcomes](#objectives--learning-outcomes)
- [Project Architecture](#project-architecture)
- [Core Components & Tasks](#core-components--tasks)
  - [Part I: Semantic Versioning (`PkgVersion`)](#part-i-semantic-versioning-pkgversion)
  - [Part II: Hash Trie & Iterator (`Trie` & `TrieIterator`)](#part-ii-hash-trie--iterator-trie--trieiterator)
  - [Part III: Local Repository (`LocalRepository`)](#part-iii-local-repository-localrepository)
  - [Part IV: Package Manager Core (`PackageManager`)](#part-iv-package-manager-core-packagemanager)
- [Build & Run Guide](#build--run-guide)
  - [Prerequisites](#prerequisites)
  - [Compilation Commands](#compilation-commands)
  - [Command Line Interface (CLI) Usage](#command-line-interface-cli-usage)
- [Testing & Sanitizers](#testing--sanitizers)
- [Submission Guidelines](#submission-guidelines)

---

## 🔍 Overview

Software package managers maintain system consistency by coordinating installations, upgrades, removals, dependency resolution, and conflict management. 

In this project, a full-featured package manager core is built using modern C++17 features (`std::variant`, `std::optional`, `std::array`, STL containers, and algorithms). The package manager ensures **all-or-nothing (atomic) operations** so that failed operations never leave the system in a broken or inconsistent state.

---

## 🎯 Objectives & Learning Outcomes

- **Modern C++17 Features:** Master type-safe sum types (`std::variant`), compile-time arrays (`std::array`), optional returns (`std::optional`), and STL algorithms.
- **Data Structures:** Implement recursive hash tries (`Trie<T, Key, N>`) with custom iterators (`TrieIterator`).
- **Bitwise Operations & Hashing:** Implement the 64-bit FNV-1a non-cryptographic hashing algorithm.
- **Dependency & Conflict Graph Operations:** Formulate graph-based transitive dependency expansion, reverse-dependency tracking, and clean removal algorithms.

---

## 📁 Project Architecture

```
assignment3/
├── Makefile                        # Build system configuration
├── include/
│   ├── expected.h                  # Result/Error monadic handler
│   ├── local_repository.h          # Installed packages container declaration
│   ├── package.h                   # Software package structure declaration
│   ├── package_manager.h           # High-level package manager declaration
│   ├── remote_repository.h         # Disk-backed remote package repo
│   ├── repository.h                # Abstract base repository class
│   ├── trie.h                      # Template Hash Trie header
│   ├── trie.tpp                     # Template Hash Trie implementation (*Submitted*)
│   ├── util.h                      # Helper utilities (parsing, strings)
│   └── version.h                   # Semantic versioning class header
├── src/
│   ├── local_repository.cpp        # Local repository implementation (*Submitted*)
│   ├── main.cpp                    # CLI entry point
│   ├── package.cpp                 # Package implementation
│   ├── package_manager-given.cpp   # Provided package manager routines
│   ├── package_manager.cpp         # Main manager logic (*Submitted*)
│   ├── repository-given.cpp        # Provided repository implementation
│   ├── util.cpp                    # Utility helper implementations
│   ├── version-given.cpp           # Provided version helpers
│   └── version.cpp                 # Version comparison logic (*Submitted*)
├── packages/                       # Sample remote package indexes
│   ├── remote/
│   │   ├── default/                # Default remote repository
│   │   └── basic/                  # Basic test repository
└── tests/                          # Suite of test cases and fixtures
    ├── fixtures.cpp / .h
    ├── given_tests.cpp / .h
    ├── main.cpp
    └── test.cpp / .h
```

---

## ⚙️ Core Components & Tasks

### Part I: Semantic Versioning (`PkgVersion`)
Represents software versions in SemVer format (`major.minor.patch`).
- **Equality Operator (`operator==`):** Evaluates exact equality across major, minor, and patch values.
- **Relational Operator (`operator<`):** Strict lexicographical ordering prioritizing major, then minor, then patch.
- **String Conversions:** `operator std::string()` formats version into string `"{major}.{minor}.{patch}"`. Overloaded output operator (`operator<<`) writes directly to output streams.

### Part II: Hash Trie & Iterator (`Trie` & `TrieIterator`)
A $N$-ary Hash Trie storing elements akin to `std::set`.
- **Node Storage:** Uses `std::variant<std::optional<std::pair<Key, T>>, std::array<Trie*, N>>` to represent either a value node or an array of child sub-trie buckets.
- **Recursive Operations:**
  - `insert`: Bubbles down existing values via `push_down` when bucket collisions occur at a specific depth.
  - `erase_from`: Prunes values and collapses empty sub-tries back up the path.
  - `size`: Computes total stored elements recursively across non-null buckets.
  - Deep copy constructor and assignment operator for clean resource duplication.
- **Iterator Traversal (`TrieIterator`):** Uses an explicit internal path stack (`std::stack<Trie*>`) to traverse the hash trie in pre-order without recursion.

### Part III: Local Repository (`LocalRepository`)
Tracks packages currently installed on the local system.
- **FNV-1a 64-bit Hashing:** Hashes package names for routing in the 17-bucket package trie using constants:
  $$hash = (hash \oplus b) \times 1099511628211$$
  with initial basis $hash = 14695981039346656037$.
- **Dual Indexing:** Maintains both a `Trie` of `InstalledPackage` instances and a reverse-dependency map (`std::map<std::string, std::vector<std::string>>`).
- **Consistency Constraints:** Syncs reverse dependencies upon `put`, `drop`, and `unlink`. Automatically erases key entries from `reverse_depends` when their dependent list becomes empty.

### Part IV: Package Manager Core (`PackageManager`)
Implements transaction-safe package lifecycle management:
- **`conflicts`:** Verifies bi-directional conflict rules between candidates and installed packages.
- **`collect_missing`:** Computes the transitive dependency expansion chain for a given package while ignoring locally satisfied dependencies.
- **`install`:** Validates dependencies and conflict checks on a local repository clone prior to committing changes. Returns status flags (`SUCCESS`, `ALREADY_INSTALLED`, `NOT_FOUND`, `CONFLICT`).
- **`uninstall`:** Safely uninstalls packages only when no installed packages rely on them, subsequently cleaning unneeded implicit dependencies.
- **`upgrade`:** Updates installed packages to newer remote versions while collecting missing dependencies introduced by updated packages.
- **`prune`:** Iteratively sweeps and removes orphan dependencies that are no longer referenced by explicitly installed software.

---

## 🚀 Build & Run Guide

### Prerequisites
- GCC/Clang with standard C++17 support (`g++ >= 9` recommended).
- GNU Make.
- Linux/macOS environment recommended for full Sanitizer support (`AddressSanitizer`, `LeakSanitizer`).

### Compilation Commands

To build all executables using parallel jobs:
```bash
make -j
```

This generates binaries in the `build/` directory:
- `build/pkg`: Non-sanitized CLI executable.
- `build/pkg.sanitized`: CLI executable compiled with ASan, LSan, and UBSan.
- `build/pkg_test`: Non-sanitized unit testing framework.
- `build/pkg_test.sanitized`: Sanitized unit testing framework.

### Command Line Interface (CLI) Usage

Run the package manager CLI:
```bash
# Display help and command options
./build/pkg --help

# Run package manager with custom local index and install a package
./build/pkg --local-index /tmp/index install <package_name>
```

---

## 🧪 Testing & Sanitizers

The project features 37 provided assertion-based unit tests.

### Running Test Cases

Run all given tests:
```bash
make test TEST_CASE=100
# or
./build/pkg_test.sanitized 100
```

Run a specific test case (e.g., Test Case 5):
```bash
make test TEST_CASE=5
```

Run tests on Windows or without memory sanitizers:
```bash
make test-unsanitized TEST_CASE=5
```

### Sanitizer Checks
All sanitized builds are compiled with:
`-fsanitize=address,leak,undefined`

Ensure that tests complete without reporting out-of-bounds reads/writes, memory leaks, or use-after-free errors.

---

## 📦 Submission Guidelines

Only the following four files are required for submission on ZINC:
1. `include/trie.tpp`
2. `src/local_repository.cpp`
3. `src/package_manager.cpp`
4. `src/version.cpp`

*Note: Avoid modifying public headers outside of specified implementation files.*