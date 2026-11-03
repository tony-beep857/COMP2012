# COMP 2012 Assignment 2: Mini-Linux File System Simulator

## Overview
This project is a simplified Linux file system simulator written in C++. It features a hierarchical directory structure, multiple users and groups management, permission handling, and a set of common Linux terminal commands. The project is designed to reinforce core Object-Oriented Programming (OOP) concepts, including inheritance, polymorphism, and manual dynamic memory management.

## Objectives
*   **Object-Oriented Design:** Implement class hierarchies using inheritance and polymorphism (e.g., abstract `FileEntry` and `Device` classes, and concrete `File`, `Directory`, `DevNull`, etc. classes).
*   **Memory Management:** Manually manage dynamic memory using `new` and `delete`, implementing deep copies and recursive destruction.
*   **Standard Library Usage:** Utilize `std::vector` for dynamic arrays and `std::optional` for handling optional return values safely.
*   **System Modeling:** Model real-world OS elements, including Linux 9-bit permissions, user/group relationships, and directory navigation.

## File System Hierarchy
The simulated file system mimics a basic Linux structure rooted at `/`:
*   `/home`: Contains home directories for regular users.
*   `/root`: The home directory for the root user.
*   `/dev`: Contains special device files (`/dev/null`, `/dev/stdin`, `/dev/stdout`).

## Supported Commands
The simulator supports simplified versions of the following Linux commands:

### User & Group Management
*   `su USER`: Switch to another user.
*   `adduser LOGIN`: Add a new user and create their home directory.
*   `groupadd GROUP`: Create a new user group (root only).
*   `usermod [-a] -G GROUP1[,GROUP2,...] LOGIN`: Modify a user's group memberships.

### File & Directory Navigation
*   `cat FILE`: Print the contents of a file to standard output.
*   `ls [-a] [-l] [-R] [FILE...]`: List directory contents (supports hidden files, long format, and recursive listing).
*   `mkdir [-p] DIRECTORY...`: Create directories (with optional parent creation).
*   `touch FILE...`: Create empty files.
*   `chmod [-R] PERMISSION FILE`: Change file or directory permissions (octal or symbolic).
*   `cp [-p] [-r|-R] SRC DEST`: Copy a file or directory.
*   `rm [-r|-R] FILE...`: Remove files and directories.
*   `cd DIRECTORY`: Change the current working directory.

## System Devices
The simulator treats devices as special files extending the `Device` class:
*   `/dev/null`: A black hole that discards all data written to it.
*   `/dev/stdin`: Standard input stream.
*   `/dev/stdout`: Standard output stream.

## Compilation and Execution

### Building the Project
To compile the project, use the provided `Makefile`. In your terminal, run:
```bash
make
```

### Running Test Cases
You can run the simulator against the provided test cases and compare the output.

**On Linux/MacOS:**
```bash
./pa2.exe < tests/test_case_number.in > test_case_number.out
diff -u tests/test_case_number.ans test_case_number.out
```

**On Windows (PowerShell):**
```powershell
Get-Content tests\test_case_number.in | .\pa2.exe
```

## Academic Integrity (Honor Code)
This project is subject to the university's Honor Code. Plagiarism, copying code, or allowing others to copy your code is strictly prohibited and monitored via plagiarism detection software.