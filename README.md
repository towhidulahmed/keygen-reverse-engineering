# KeygenMe3 Reverse Engineering

Reverse engineering the keygenme3 binary and creating a keygen.

## Overview

The keygenme3 binary asks for an email address and serial number. This project documents how I reverse engineered it and created a working keygen.

## Repository Contents

```
├── binary/keygenme3      # Original binary
├── src/cracker.c         # Keygen implementation
├── src/main.c            # Reimplementation of the binary
├── src/varify.c          # Verification tool
└── images/               # Ghidra screenshots
```

## Quick Start

```bash
make cracker
./bin/cracker pr0cracker
```

Output: `1f68-190d9`

Test it:
```bash
./binary/keygenme3
# Enter: pr0cracker
# Enter: 1f68-190d9
# Result: Correct serial! Software unlocked.
```

## Reverse Engineering Process

### Step 1: Binary Analysis

Used `file` command to check the binary type:

```bash
$ file keygenme3
keygenme3: ELF 64-bit LSB executable, x86-64, version 1 (SYSV),
dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2,
BuildID[sha1]=84fc4344dc69402219a6f4cbfe4c04fa00961ff1, 
for GNU/Linux 3.2.0, stripped
```

The binary is stripped, meaning no symbol names or debug info.

### Step 2: Ghidra Analysis

Opened the binary in Ghidra and let it auto-analyze.

Since it was stripped, functions had generic names like `FUN_00101234`. I renamed them:
- Entry point function → `main`
- Validation function → `serial_check`

### Step 3: Function Analysis

Found the `serial_check` function that validates the serial:

```c
bool serial_check(char *email, char *serial)
{
  int iVar1;
  size_t sVar2;
  uint local_44;
  uint local_40;
  int local_3c;
  char local_38[24];
  
  local_44 = 0;
  local_40 = 0;
  local_3c = 0;
  
  while (true) {
    sVar2 = strlen(email);
    if (sVar2 <= (ulong)(long)local_3c) break;
    
    local_44 = local_44 + (int)email[local_3c] * 8;
    local_40 = (local_40 + (int)email[local_3c] * (int)email[local_3c]) - 0xbc;
    local_3c = local_3c + 1;
  }
  
  sprintf(local_38, "%04x-%04x", local_44, local_40);
  iVar1 = strcmp(serial, local_38);
  return iVar1 == 0;
}
```

### Step 4: Algorithm Discovery

The algorithm is simple:
- For each character in the email:
  - `local_44` accumulates `character * 8`
  - `local_40` accumulates `character² - 0xbc` (0xbc = 188 in decimal)
- Format both as 4-digit hex numbers separated by a dash

### Step 5: Implementation

Created `cracker.c` with the same algorithm:

```c
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <username>\n", argv[0]);
        return 1;
    }
    
    char *email = argv[1];
    unsigned int local_44 = 0;
    unsigned int local_40 = 0;
    
    for (int i = 0; i < strlen(email); i++) {
        local_44 += email[i] * 8;
        local_40 += (email[i] * email[i]) - 0xbc;
    }
    
    printf("%04x-%04x\n", local_44, local_40);
    return 0;
}
```

### Step 6: Testing

```bash
$ ./cracker pr0cracker
1f68-190d9

$ ./keygenme3
********************************
E-Mail address: pr0cracker
Serial number: 1f68-190d9
Correct serial! Software unlocked.
********************************
```

Works!

## The Algorithm

```
For each character in the email:
    local_44 += character * 8
    local_40 += (character * character) - 0xbc

serial = sprintf("%04x-%04x", local_44, local_40)
```

Example for "pr0cracker":
- local_44 = 8040 = 0x1f68
- local_40 = 102617 = 0x190d9
- Serial: 1f68-190d9

## Building

```bash
make all        # Build everything
make cracker    # Build just the keygen
make clean      # Clean build files
```

## Files

- **cracker.c** - Generates valid serials
- **main.c** - Full reimplementation of the original binary
- **varify.c** - Verification tool (has a bug, sprintf line is commented out)

## Notes

Variable names like `local_44`, `local_40`, etc. come from Ghidra. They represent stack offsets. I kept them to match the analysis.

## Screenshots

The `images/` folder contains:
- `main_fun_ghidra.png` - Main and serial_check functions in Ghidra
- `serial_chk_ghidra.png` - Close-up of serial_check function
- `run_keygenme3.png` - Running the original binary
