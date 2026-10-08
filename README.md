# File Search Processor

File Search Processor (FSP) is a lightweight C++ utility for searching text patterns across files in a directory tree. It is designed as a small multithreaded search tool, with a queue-based pipeline, a configurable worker pool, and runtime statistics reporting.

The project is intended as a learning and demonstration project for file traversal, concurrency, CLI parsing, and basic search orchestration in C++20.

## Features

- Recursive directory scanning
- Text pattern search across files
- Multithreaded file processing using a thread pool
- Flexible CLI arguments for pattern, root path, and thread count
- Optional runtime statistics output
- Unit tests for parsing and processing behavior

## Project goals

The application is built around a simple pipeline:

1. Traverse a root directory recursively.
2. Enqueue discovered files.
3. Process files in parallel through a worker pool.
4. Search for a text pattern in each file.
5. Return the first matching result and optionally print execution statistics.

## Requirements

- CMake 3.20 or newer
- C++20 compatible compiler
- Linux environment (the project is built and tested on Linux)
- Internet access during build so CMake can fetch dependencies automatically

The project uses the following dependencies fetched by CMake:

- bs_thread_pool (Git: https://github.com/bshoshany/thread-pool)
- spdlog (Git: https://github.com/gabime/spdlog)
- Catch2 (Git: https://github.com/catchorg/Catch2) for unit tests

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

To build only the application binary:

```bash
cmake --build build --target fsp
```

To build the test target:

```bash
cmake --build build --target fsp_unit_tests
```

## Usage

### Basic syntax

```bash
./build/fsp <pattern> <path>
./build/fsp -p <pattern> -r <path> [-t <threads>] [-s]
```

### Supported arguments

- `-p`, `--pattern` — search pattern to match
- `-r`, `--root` — root path to scan
- `-t`, `--threads` — maximum number of worker threads
- `-s`, `--stats`, `--statistics` — print runtime statistics
- `-R`, `--recursive` — enable recursive directory scanning (flag, no value). Can be used together with `-s` or without it.
- `-h`, `--help` — show usage information

### Examples

Search for the word `needle` under the demo directory:

```bash
./build/fsp -p needle -r demo_root
```

Search using a custom thread count and enable statistics output:

```bash
./build/fsp -p needle -r demo_root -t 4 -s
```

Search recursively (flag `-R`) without statistics:

```bash
./build/fsp -p needle -r demo_root -R
```

Search recursively with statistics:

```bash
./build/fsp -p needle -r demo_root -R -s
```

Search using positional arguments:

```bash
./build/fsp needle demo_root
```

## Example output

```text
[17:29:06 +03:00] [info] Search result:
[17:29:06 +03:00] [info] line_number = -1
[17:29:06 +03:00] [info] text =
[17:29:06 +03:00] [info] file_name =
[17:29:06 +03:00] [info] ==================================================
[17:29:06 +03:00] [info] Statistics summary
[17:29:06 +03:00] [info] --------------------------------------------------
[17:29:06 +03:00] [info] files_scanned = 5
[17:29:06 +03:00] [info] files_matched = 0
[17:29:06 +03:00] [info] files_with_errors = 0
[17:29:06 +03:00] [info] directories_scanned = 3
[17:29:06 +03:00] [info] matched_lines = 0
[17:29:06 +03:00] [info] elapsed_ms = 1
[17:29:06 +03:00] [info] ==================================================
```

## Project structure

```text
.
├── CMakeLists.txt
├── main.cpp
├── README.md
├── docs/
├── fsp/
│   ├── cli/
│   ├── concurrency/
│   ├── filesystem/
│   ├── scanners/
│   ├── search/
│   ├── services/
│   └── statistics/
├── tests/
│   ├── unit/
│   └── utils/
└── build/
```

## Main modules

- `fsp/cli` — command-line parsing and configuration handling
- `fsp/scanners` — directory and file scanning logic
- `fsp/services` — orchestration of the search pipeline
- `fsp/search` — search engine and result model logic
- `fsp/statistics` — runtime counters and elapsed time tracking
- `fsp/filesystem` — file reading and file-system abstractions

## Testing

To run the unit test suite:

```bash
cd build
ctest --output-on-failure
```

Or run the binary directly:

```bash
./build/tests/fsp_unit_tests
```

## Notes

This project is intentionally small and easy to follow. It demonstrates how to combine:

- CLI parsing,
- multithreaded work distribution,
- file traversal,
- pattern matching,
- basic statistics collection,
- and testable application structure.

It is suitable for experiments, teaching, and extension into a fuller production-grade search utility.

## License

This project is distributed under the terms of the repository license. See the project root for the full license text.
