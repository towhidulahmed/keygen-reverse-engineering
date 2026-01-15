# Quick Start

Get the keygen running in 2 minutes.

## Build

```bash
make cracker
```

## Run

```bash
./bin/cracker pr0cracker
```

Output: `1f68-190d9`

## Test

```bash
./binary/keygenme3
```

Enter:
- Email: `pr0cracker`
- Serial: `1f68-190d9`

Result: `Correct serial! Software unlocked.`

## Build Everything

```bash
make all
```

This builds:
- `bin/cracker` - The keygen
- `bin/main` - Reimplementation of the original
- `bin/varify` - Verification tool

## Use with Different Email

Edit `src/cracker.c` line 6:

```c
char *email = "your_email_here";
```

Rebuild:

```bash
make cracker
./bin/cracker
```

## Clean

```bash
make clean
```

## Files

- `README.md` - Main documentation
- `GHIDRA_NOTES.md` - Ghidra analysis process
- `ANALYSIS.md` - Technical details
- `BUILD.md` - Detailed build instructions
