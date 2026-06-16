# Assembler

A two-pass assembler for a custom assembly language, written in C. Final project for a Systems Programming course.

Takes `.as` source files and outputs object files, along with `.ent` / `.ext` files if the source uses entry/extern directives.

## How it works

Processing happens in three stages:

- **Pre-processor** – expands macros, strips comments and blank lines. Produces a `.am` file.
- **First pass** – builds the symbol table, validates instructions and operands, encodes data/string directives. Instructions are partially encoded here since labels might not be defined yet.
- **Second pass** – resolves all label references, finishes encoding, writes output files.

## File overview

```
assembler.c       - main, runs the pipeline per input file
pre_proccess.c    - macro expansion
first_pass.c      - symbol collection, partial encoding
second_pass.c     - symbol resolution, output
symbols.c         - symbol table (linked list)
machine_image.c   - code/data image buffers
output.c          - writes .ob, .ent, .ext
const_tables.c    - instruction + register tables
string_utils.c    - parsing helpers
utils.c           - error reporting, misc
structs.h         - core structs (file_data, symbol, ...)
constants.h       - constants and enums
```

## Some implementation notes

- 12-bit word size, 4096 words of memory, code starts at address 100
- Addressing modes: immediate (`#n`), direct (label), relative (`&label`), register (`r0`-`r7`)
- The symbol table is a linked list — fine for the scale of this project, but a hash map would be better for larger inputs
- If a file has errors, the assembler reports them all and moves on to the next file instead of stopping

## Build & run

```bash
make
./assembler program        # looks for program.as
./assembler file1 file2    # supports multiple files
make clean
```
