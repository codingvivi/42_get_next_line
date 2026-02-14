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

### Reference documents
The `reference/` folder
contains all musl code I annotated for easier understanding
plus my math notes,
the flowchart,
and the source code of either

## Instructions
### Building
If cloned from my github,
this project has premake as an optional dependency,
which will generate makefiles automatically.

Once installed,
run
```bash
    git clone https://github.com/codingvivi/42_get_next_line
    cd 42_get_next_line/
    git submodule update --init --recursive
    premake5 gmake # to generate make file
    premake5 ecc #to generate record of compile flags for linting
```

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

*note: due to time constraints, I didnt end up using the virtualization the 42 configurations use much, so I won't vouch for them working.*

### Algorithm
The rough outline of the algorithm is as follows
(Only visible in my personal repo,
github backup [here](https://github.com/codingvivi/42_get_next_line/blob/main/reference/docs/program_flow.svg)):
![Flow of the program](./reference/docs/program_flow.svg)


#### Philosophy
Since musl usually goes for optimized,
widely compatible code,
I tried to mirror their approach
as much as the norm would allow me.

The aforementioned optimized approach is reflect in:

##### Few memory and copy operations
Copies and mallocs are kept to a minimum.
No costly linked lists,
no temp buffers that I didnt feel like I absolutely needed.
If no delimiter is found on read
the buffer to be returned is grown geometrically
and then shrunk back down to exact size once before return,
this should help with long lines smaller READBUFFER sizes.

##### 1.5 growth factor
The growth-factor for the read buffer is set to 1.5,
famously used by Facebook
in their C++ std::vector implementation,
it should be a good compromise
between allocating more memory in advance,
curbing external fragmentation of the freed,
previous memory
and being a sane ratio for the computer to work with.
The notes I took
working through the proof of this number
are found [here](./reference/docs/growth_proof.pdf)
(github [backup](https://github.com/codingvivi/42_get_next_line/blob/main/reference/src/growth_proof.typ))

##### Optimized read loop
The main while loop is an endless one loop,
checking and breaking is done manually.
This should be more efficient
than checking if some semantically meaningful
but complex break condition is true constantly,
even during the copying of every byte.

##### Bitwise flags (for fun)
Bitwise operations are used
to set flags
that store operations about the read,
as they would be when using an actual FILE.
I can't imagine the compiler not doing this
when it's optimizing,
but I did it anyway
just for the sake of the exercise.

## References
[1] D. Lemire, "Appending to an std::string character-by-character: how does the capacity grow?," *Daniel Lemire's blog*. Accessed: Feb. 12, 2026. [Online]. Available: https://lemire.me/blog/2023/10/23/appending-to-an-stdstring-character-by-character-how-does-the-capacity-grow/

[2] "Computer data storage," *Wikipedia*. Feb. 12, 2026. Accessed: Feb. 12, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Computer_data_storage&oldid=1337916484#Primary

[3] "Computer memory," *Wikipedia*. Feb. 01, 2026. Accessed: Feb. 12, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Computer_memory&oldid=1335951825

[4] "folly/folly/docs/FBVector.md at main · facebook/folly," *GitHub*. Accessed: Feb. 12, 2026. [Online]. Available: https://github.com/facebook/folly/blob/main/folly/docs/FBVector.md

[5] "Fragmentation (computing)," *Wikipedia*. Apr. 21, 2025. Accessed: Feb. 12, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Fragmentation_(computing)&oldid=1286753577#Types

[6] F. Domingues and P. Singh, *francinette*. (Feb. 05, 2026). C. Accessed: Feb. 12, 2026. [Online]. Available: https://github.com/Prashant-Bharaj/francinette

[7] Kay Lack, *In search of the perfect dynamic array growth factor*, (Jan. 24, 2025). Accessed: Feb. 14, 2026. [Online Video]. Available: https://www.youtube.com/watch?v=GZPqDvG615k

[8] R. Felker, *musl libc*. (Feb. 29, 2024). C. Accessed: Feb. 12, 2026. [Online]. Available: https://git.musl-libc.org/cgit/musl/tag/?h=v1.2.5

[9] 262588213843476, "Optimal memory reallocation and the golden ratio," *Gist*. Accessed: Feb. 12, 2026. [Online]. Available: https://gist.github.com/jemmanuel/b8277e7922e9b9947e2f171cc85f1d01

[10] sarahmarienothling, "Optimising the growth factor of a dynamic Array List," *Sarah-Marie Nothling*. Accessed: Feb. 12, 2026. [Online]. Available: https://sarahnothling.wordpress.com/2015/06/04/optimising-the-growth-factor-of-a-dynamic-array-list/

[11] BitLemon, *Page Tables and MMU: How Virtual Memory Actually Works Behind the Scenes (Animation)*, (Mar. 03, 2025). Accessed: Feb. 12, 2026. [Online Video]. Available: https://www.youtube.com/watch?v=B6tJxvYBNrU

[12] M. Bystrin, *premake-ecc*. (Feb. 04, 2026). Lua. Accessed: Feb. 12, 2026. [Online]. Available: https://github.com/MattBystrin/premake-ecc

[13] *premake/premake-core*. (Feb. 12, 2026). C. Premake. Accessed: Feb. 12, 2026. [Online]. Available: https://github.com/premake/premake-core

[14] "Random-access memory," *Wikipedia*. Jan. 30, 2026. Accessed: Feb. 14, 2026. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Random-access_memory&oldid=1335683491

[15] BitLemon, *Virtual Memory Explained (including Paging)*, (Feb. 06, 2025). Accessed: Feb. 12, 2026. [Online Video]. Available: https://www.youtube.com/watch?v=fGP6VHxqkIM

### AI usage
Google Gemini was used,
both Pro and Flash,
both version 3.0 and 2.5,
to:
- search through the premake documentation faster
- find some bugs that me and no one else I asked for help caught
- freshen up my knowledge on infinite series
- Write a first draft for the readme
- convert Zoteros bibliography export to markdown while keeping the IEEE formatting

WolframAlpha's equation solver
was used to calculate the final quadratic equation
of the golden ratio proof.
