
**Course:** CMSC 326 - Operating Systems  
**Group:** Mykhailo Pryshchepa & Zuhayr Abdullazhanov  
**Repository:** https://github.com/loyaldept/Pintos-Alarms.git

## Overview

This project implements an efficient alarm clock system for the Pintos operating system. The implementation replaces busy-waiting in `timer_sleep()` with a proper blocking mechanism that allows other threads to run while threads are sleeping.

## Key Features

- **Efficient Sleep List**: Maintains sleeping threads in a sorted list by wakeup time
- **Thread Safety**: Uses mutex locks to prevent race conditions
- **Optimized Interrupt Handler**: Minimizes time spent in timer interrupt handler
- **Proper Synchronization**: Handles concurrent access and timer interrupts safely

## Project Structure

```
Pintos-Alarms/
├── devices/
│   ├── timer.c          # Main alarm implementation
│   ├── timer.h          # Timer interface
│   └── Makefile
├── threads/
│   ├── thread.c         # Modified thread implementation
│   ├── thread.h         # Thread structure with wakeup_tick
│   ├── synch.h          # Synchronization primitives
│   └── Makefile
├── tests/
│   └── threads/
│       ├── alarm-single.c    # Single alarm test
│       ├── alarm-multiple.c  # Multiple alarms test
│       ├── alarm-negative.c  # Negative sleep test
│       ├── alarm-zero.c      # Zero sleep test
│       └── alarm-busy.c      # Busy waiting test
├── lib/                 # Debug utilities
├── DESIGN_DOCUMENT.md    # Complete design documentation
└── Makefile             # Main build file
```

## Building the Project

### Prerequisites
- GCC compiler
- Make
- Pintos development environment

### Build Commands

```bash
# Navigate to project directory
cd /path/to/Pintos-Alarms

# Clean previous builds
make clean

# Build the entire project
make

# Build specific components
make -C threads
make -C devices
make -C lib
```

## Running Tests

### Individual Test Execution

```bash
# Run single alarm test
cd tests/threads
make alarm-single
pintos -- -q run alarm-single

# Run multiple alarms test
make alarm-multiple
pintos -- -q run alarm-multiple

# Run negative sleep test
make alarm-negative
pintos -- -q run alarm-negative

# Run zero sleep test
make alarm-zero
pintos -- -q run alarm-zero

# Run busy waiting test
make alarm-busy
pintos -- -q run alarm-busy
```

### Running All Tests

```bash
# Run all alarm tests
cd tests/threads
make check TESTS=alarm-*

# Or run specific tests
make check TESTS=alarm-single alarm-multiple alarm-negative alarm-zero alarm-busy
```

### Expected Test Results

All tests should **PASS** with the following expected outputs:

- **alarm-single**: Tests single thread sleeping for 5 ticks
- **alarm-multiple**: Tests multiple consecutive sleeps (3 + 5 = 8 ticks total)
- **alarm-negative**: Tests that negative sleep values don't sleep
- **alarm-zero**: Tests that zero sleep values don't sleep
- **alarm-busy**: Tests that sleeping threads don't consume CPU

## Implementation Details

### Key Data Structures

```c
// Global sleep list (in timer.c)
static struct list sleep_list;           // Sorted list of sleeping threads
static struct lock sleep_list_lock;      // Mutex for thread safety

// Thread structure addition (in thread.h)
struct thread {
    // ... existing fields ...
    int64_t wakeup_tick;                // When this thread should wake up
};
```

### Core Algorithm

1. **timer_sleep(ticks)**:
   - Calculate wakeup time: `wakeup_tick = current_tick + ticks`
   - Disable interrupts and acquire lock
   - Insert thread into sorted sleep list
   - Block the thread and release lock
   - Re-enable interrupts

2. **Timer Interrupt Handler**:
   - Increment global tick counter
   - Scan sleep list for threads ready to wake up
   - Unblock ready threads and remove from sleep list
   - Yield to scheduler

### Synchronization Strategy

- **Mutex Lock**: Protects sleep list from concurrent access
- **Interrupt Disabling**: Prevents race conditions during critical sections
- **Sorted List**: Enables efficient interrupt handler with early termination

## Troubleshooting

### Common Issues

1. **Build Errors**:
   ```bash
   # Clean and rebuild
   make clean
   make
   ```

2. **Test Failures**:
   - Check that `wakeup_tick` field is properly initialized in `thread_create()`
   - Verify lock is acquired before accessing sleep list
   - Ensure interrupts are properly disabled/enabled

3. **Infinite Loops**:
   - Verify `thread_block()` is called after adding to sleep list
   - Check that `thread_unblock()` is called in interrupt handler

### Debug Commands

```bash
# Run with debug output
pintos -- -q run alarm-single -v

# Check for memory leaks
valgrind pintos -- -q run alarm-single

# Run with additional debugging
pintos -- -q run alarm-single --gdb
```

## Performance Characteristics

- **Sleep List Insertion**: O(n) where n = number of sleeping threads
- **Interrupt Handler**: O(k) where k = number of threads ready to wake up
- **Memory Usage**: O(n) additional space for sleep list
- **CPU Efficiency**: Eliminates busy-waiting, allowing other threads to run

## Design Rationale

The chosen design prioritizes:
- **Correctness**: Clear synchronization model with well-defined critical sections
- **Simplicity**: Easy to understand, implement, and debug
- **Efficiency**: Reasonable performance for typical Pintos workloads
- **Maintainability**: Straightforward code for educational purposes

## References

- Pintos Documentation: https://pintos-os.org/
- Operating Systems Concepts
- KAIST OS Lab Pintos Materials
- Course Lecture Notes
