_This project has been created as part of the 42 curriculum by lrain_

# Get Next Line

## Description
This project implements the `get_next_line` function,
which returns a line from a file descriptor,
ending with a newline character or EOF.

The goal of this project was to understand file handling,
buffer management,
and static variables in C programming.

Just like the previous project,
where I also was tasked with writing low level help code
to be added to a general purpose C library,
I decided to once again use musl
as my implementation reference,
last time I did I ended up learning a lot.


### Algorithm
The rough outline of the algorithm is as follows
![Flow of the program](./reference/docs/program_flow.svg)

### Philosophy
Since musl usually goes for optimized,
widely compatible code,
I tried to mirror their approach
as much as the norm would allow me.

## Optimizations
The afformentioned optimized approach is reflect in:

### Few memory and copy operations
Copies and mallocs are kept to a minimum.
No costly linked lists,
no temp buffers that I didnt feel like I absolutely needed.
If no delimiter is found on read
the buffer to be returned is grown geometrically
and then shrunk back down to exact size once before return,
this should help with long lines smaller READBUFFER sizes.
Furthermore, the growth-factor is set to 1.5,
famously used by Facebook
in their C++ std::vector implementation,
this should curb external fragmentation of the freed memory.

### Optimized read loop
The main while loop is an endless one loop,
checking and breaking is done manually.
This should be more efficient
than checking if some semantically meaningful
but complex break condition is true constantly,
even during the copying of every byte.

### Bitwise flags (for fun)
Bitwise operations are used
to set flags
that store operations about the read,
as they would be when using an actual FILE.
I can't imagine the compiler not doing this
when it's optimizing,
but I did it anyway
just for the sake of the exercise.


## Instructions
If cloned from my github,
this project has premake as an optional dependency,
which will generate makefiles automatically.

Once installed,
run
```
    git clone https://github.com/codingvivi/42_get_next_line
    cd 42_get_next_line/
    git submodule update --init --recursive
    premake5 gmake # to generate make file
    premake5 ecc #to generate record of compile flags for linting
```

### Build instructions
Once generated with premake,
the compile options are:
```
> make help
Usage: make [config=name] [target]

CONFIGURATIONS:
  release_dev
  release_42
  debug1b_dev
  debug1b_42
  debug10b_dev
  debug10b_42
  debug1mb_dev
  debug1mb_42

TARGETS:
   all (default)
   clean
   get_next_line
   test_runner


```
- **Configurations**:
    - `Release`: A standard release build with no debugging information.
    - `Debug1b`: A debug build that defines `BUFFER_SIZE` as 1. This is useful for testing the `get_next_line` function with a small buffer.
    - `Debug10b`: A debug build that defines `BUFFER_SIZE` as 10.
    - `Debug1mb`: A debug build that defines `BUFFER_SIZE` as 1,000,000. This is useful for testing with very large lines.

- **Platforms**:
    - `Dev`: Configured for my machine, which is ARM64 linux.
    - `42`: Configured for the 42 school environment, targeting x86_64 architecture on Linux.

- **Projects**:
    - `get_next_line`: The static library that contains the `get_next_line` function and its utilities.
    - `test_runner`: This is a console application that links against the `get_next_line` library for testing purposes.

With no arguments supplied,
running `make` will build library and runner
in the release_dev configuration.

_note: due to time constraints,
I didnt end up using the virtualization
the 42 configurations use much,
so I won't vouch for them working.
_


## Detailed Explanation

## Resources
*(This section has been intentionally left blank)*
