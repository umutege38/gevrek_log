# GevrekLog

**GevrekLog** is a lightweight, thread-safe, and debug-only logging and error-handling library written in pure **C11**. It is designed to be minimal, fast, and completely compile-out in Release mode for zero overhead in production.

---

## Features

- **Debug-Only Logging:** Logging functionality and macros compile down to no-ops (`((void)0)`) in Release mode.
- **Thread-Local Error State:** Safely store and retrieve engine errors per thread using `_Thread_local` storage.
- **Flexible Sinks:** Built-in console output (stdout/stderr) with support for custom log callbacks and user contexts.
- **Severity Levels:** `INFO`, `WARNING`, `ERROR`, and `FATAL`.
- **Compile-Time Assertions:** Portable `GEVREK_STATIC_ASSERT` macro supporting C11 `_Static_assert` with safe fallback for older environments.
- **Pure C11:** Zero external dependencies and clean modern C implementation.

---

## Installation & Build

GevrekLog uses CMake. You can build it as a static library:

    mkdir build && cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cmake --build .

---

## API Reference

### Logging (_DEBUG Mode Only)

| Function / Macro | Description |
| :--- | :--- |
| GEVREK_LOG(severity, fmt, ...) | Formatted logging macro. Automatically compiles to a no-op in Release mode. |
| GevrekSetLogLevel(GevrekLogSeverity level) | Sets the minimum global threshold for logging (GEVREK_LOG_INFO, _WARNING, _ERROR, _FATAL). |
| GevrekGetLogLevel(void) | Returns the current active log level. |
| GevrekLogSetCallback(GevrekLogCallback cb, void *ctx) | Redirects logs from standard output to a custom callback function. |

### Error Handling (Always Available)

| Function | Description |
| :--- | :--- |
| GevrekSetError(GevrekError error, const char *msg) | Sets the thread-local error code and descriptive message string. |
| GevrekGetLastError(void) | Retrieves the current thread's GevrekError code. |
| GevrekGetErrorString(GevrekError error) | Returns a static string description for a given error code enum. |
| GevrekGetLastErrorMessage(void) | Retrieves the current thread's detailed error message string buffer. |

### Error Codes (GevrekError)

- GEVREK_ERROR_NONE (0)
- GEVREK_ERROR_OUT_OF_MEMORY
- GEVREK_ERROR_FILE_IO
- GEVREK_ERROR_INVALID_PARAMETER
- GEVREK_ERROR_DEVICE_FAILURE
- GEVREK_ERROR_UNKNOWN

### Compile-Time Assertions

| Macro | Description |
| :--- | :--- |
| GEVREK_STATIC_ASSERT(cond, msg) | Asserts a condition at compile time. Uses native _Static_assert on C11+ or a safe array-size fallback on legacy compilers. |

---

## Quick Start

### 1. Logging

    #include "gevrek_log.h"

    int main(void) {
        GevrekSetLogLevel(GEVREK_LOG_INFO);

        GEVREK_LOG(GEVREK_LOG_INFO, "Engine initialized successfully.");
        GEVREK_LOG(GEVREK_LOG_WARNING, "Low memory warning: %d MB remaining", 64);
        
        return 0;
    }

### 2. Thread-Local Error Handling

    #include "gevrek_log.h"

    void load_resource() {
        if (/* allocation fails */ 1) {
            GevrekSetError(GEVREK_ERROR_OUT_OF_MEMORY, "Failed to allocate memory arena");
            return;
        }
    }

    // Checking errors from any thread safely:
    if (GevrekGetLastError() != GEVREK_ERROR_NONE) {
        printf("Error: %s\n", GevrekGetLastErrorMessage());
    }

---
