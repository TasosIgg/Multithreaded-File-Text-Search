# Multithreaded File Text Search

A C++ application for searching text across many files in parallel, using multithreading and a thread‐pool architecture.

---

## Table of Contents

1. [Overview](#overview)  
2. [Features](#features)  
3. [Repository Structure](#repository-structure)  
4. [Design & Architecture](#design--architecture)  
   - ThreadPool  
   - SearchWorker  
   - SearchManager  
   - FileUtils  
   - GUI (SearchWindow)  
5. [Building](#building)  
6. [Usage / Running](#usage--running)  

---

## Overview

This project implements a multi-threaded file content search utility in C++. Given a root directory (or a set of files) and a search string (or pattern), it traverses the directory tree, reads files, and finds occurrences of the search query in a parallel fashion using worker threads.

Its main motivation is to speed up text search operations over large file sets by using concurrency, rather than doing everything in a single thread.

---

## Features

- Directory traversal to locate files to scan  
- A thread pool to manage and reuse worker threads  
- Workers read file contents and perform search operations  
- GUI (via `SearchWindow`) to allow interactive searches  
- Modular components: file utilities, manager, workers, threading  
- Configurable concurrency  

---

## Repository Structure

```
.
├── CMakeLists.txt
├── main.cpp
├── ThreadPool.h / ThreadPool.cpp
├── SearchWorker.h / SearchWorker.cpp
├── SearchManager.h / SearchManager.cpp
├── FileUtils.h / FileUtils.cpp
├── SearchWindow.h / SearchWindow.cpp
└── README.md
```

- **CMakeLists.txt** — build configuration  
- **main.cpp** — entry point  
- **ThreadPool** — a generic thread-pool implementation  
- **SearchWorker** — encapsulates a job that reads a file and searches text  
- **SearchManager** — coordinates directory traversal and dispatching tasks  
- **FileUtils** — file / path utility functions (e.g. reading directory contents)  
- **SearchWindow** — GUI interface component to accept input and display results  

---

## Design & Architecture

### ThreadPool

- Manages a fixed (or dynamic) number of worker threads  
- Accepts tasks (callables or functors) and distributes them to threads  
- Maintains a task queue and worker threads wait for new tasks  

### SearchWorker

- Represents a unit of work: open a file, read its content (or line by line), search for the target text  
- Returns matches (e.g. line numbers, or context) to the manager  
- Handles errors (file open failures, IO exceptions)  

### SearchManager

- Walks the directory structure (recursively) using `FileUtils`  
- For every relevant file found, submits a `SearchWorker` task to the ThreadPool  
- Aggregates results and sends them to the UI or console output  
- Manages lifecycle: starting tasks, waiting for all tasks to finish  

### FileUtils

- Utility functions for filesystem operations:  
  - Listing files in a directory  
  - Checking file types / filtering by extension  
  - Combining / normalizing paths  
  - Reading file contents or streaming lines  

### SearchWindow (GUI component)

- Provides a user interface (window) for entering search queries, selecting the root path, and viewing results  
- Hooks into `SearchManager` to trigger searches on user action  
- Displays results (e.g. files + matching lines)  

---

## Building

This project uses **CMake** as its build system. You can build it with the following steps:

```bash
# from repository root
mkdir build
cd build
cmake ..
cmake --build .   # or: make, or your preferred build tool
```

You may need to ensure certain dependencies (e.g. a GUI framework, standard C++ libs) are available on your system.

If you want to build only a console (no GUI) version, you might need to disable or stub out the `SearchWindow` component (depending on how the project is structured).

---

# Usage / Running

This will search through all files under `/home/user/documents` for the string `"TODO"`.

If using the GUI:

1. Launch the GUI executable (e.g. `MultithreadedFileTextSearch` without arguments)  
2. Enter/select the root folder  
3. Enter the search string  
4. Press "Search"  
5. View results in the UI  

---
