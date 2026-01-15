# Build Instructions

How to compile and run the tools.

## Prerequisites

You need GCC:

```bash
gcc --version
```

If not installed:
- macOS: `xcode-select --install`
- Ubuntu/Debian: `sudo apt-get install build-essential`
- Fedora: `sudo dnf install gcc`

## Building

### Using Make

```bash
make all        # Build everything
make cracker    # Build only the keygen
make main       # Build only the reimplementation
make varify     # Build only the verification tool
make clean      # Remove all binaries
```

### Manual Compilation

```bash
gcc src/cracker.c -o bin/cracker
gcc src/main.c -o bin/main
gcc src/varify.c -o bin/varify
```

## Running

### The Keygen

```bash
./bin/cracker pr0cracker
```

Output: `1f68-190d9`

### The Reimplementation

```bash
./bin/main
```

Interactive - prompts for email and serial.

### The Original Binary

```bash
chmod +x binary/keygenme3
./binary/keygenme3
```

Interactive - prompts for email and serial.

## Modifying the Keygen

To generate serials for different emails, edit `src/cracker.c`:

```c
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <username>\n", argv[0]);
        return 1;
    }
    
    char *email = argv[1];  // Takes email from command line
    // ... rest of code
}
```

Then use it like:

```bash
./bin/cracker anyemail@example.com
```

## Troubleshooting

### Permission Denied

```bash
chmod +x binary/keygenme3
chmod +x bin/cracker
```

### Make Not Found

Install make:
- Ubuntu: `sudo apt-get install make`
- macOS: Included with Xcode tools

### GCC Not Found

See Prerequisites section above.

## File Structure

```
src/
  cracker.c     - Keygen source
  main.c        - Reimplementation source
  varify.c      - Verification source

bin/
  cracker       - Compiled keygen
  main          - Compiled reimplementation
  varify        - Compiled verification

binary/
  keygenme3     - Original binary
```
