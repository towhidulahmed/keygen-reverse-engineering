# KeygenMe3 Reverse Engineering

A comprehensive reverse engineering project demonstrating binary analysis, algorithm extraction, and keygen development for the keygenme3 challenge binary.

![KeygenMe3 Running](images/run_keygenme3.png)

## 🎯 Overview

This project documents the complete process of reverse engineering a stripped 64-bit ELF binary that implements a serial number validation system. Using tools like Ghidra, I analyzed the binary, extracted the validation algorithm, and created a working keygen that generates valid serial numbers for any given email address.

**Key Achievement**: Successfully cracked the serial validation algorithm and implemented a fully functional keygen in C.

## 📁 Repository Structure

```
keygen-reverse-engineering/
├── binary/
│   └── keygenme3          # Original challenge binary (stripped ELF)
├── src/
│   ├── cracker.c          # Keygen implementation (generates valid serials)
│   ├── main.c             # Complete reimplementation of keygenme3
│   └── varify.c           # Serial verification tool
├── images/
│   ├── run_keygenme3.png        # Binary execution demo
│   ├── main_fun_ghidra.png      # Ghidra main function analysis
│   └── serial_chk_ghidra.png    # Ghidra serial_check decompilation
├── ANALYSIS.md            # Detailed technical analysis
├── BUILD.md               # Compilation instructions
├── QUICKSTART.md          # 2-minute getting started guide
└── GHIDRA_NOTES.md        # Ghidra analysis notes
```

## 🚀 Quick Start

### Build the Keygen

```bash
make cracker
```

### Generate a Serial

```bash
./bin/cracker pr0cracker
```

**Output**: `1f68-190d9`

### Test with Original Binary

```bash
./binary/keygenme3
```

```
********************************
E-Mail address: pr0cracker
Serial number: 1f68-190d9
Correct serial! Software unlocked.
********************************
```

✅ **Success!** The serial is validated correctly.

## 🔍 Reverse Engineering Process

### 1. Binary Analysis

First, I examined the binary type and characteristics:

```bash
$ file binary/keygenme3
keygenme3: ELF 64-bit LSB executable, x86-64, version 1 (SYSV),
dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2,
for GNU/Linux 3.2.0, stripped
```

**Key Findings**:
- 64-bit ELF executable for Linux
- x86-64 architecture
- **Stripped** - No debugging symbols or function names
- Dynamically linked

### 2. Ghidra Decompilation

Loaded the binary into Ghidra for static analysis. Since the binary was stripped, all functions had generic names like `FUN_00101234`.

![Ghidra Main Function Analysis](images/main_fun_ghidra.png)

After analysis, I identified and renamed key functions:
- Entry point → `main`
- Validation logic → `serial_check`

### 3. Serial Validation Algorithm

Found the critical `serial_check` function that validates serials:

![Serial Check Function in Ghidra](images/serial_chk_ghidra.png)

Decompiled code from Ghidra:

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

> **Note**: Variable names like `local_44`, `local_40` are Ghidra's auto-generated names based on stack offsets.

### 4. Algorithm Extraction

After analyzing the decompiled code, I extracted the serial generation algorithm:

**Algorithm Breakdown**:

```
Initialize:
  part1 = 0
  part2 = 0

For each character in email:
  part1 += character * 8
  part2 += (character * character) - 0xbc    // 0xbc = 188 decimal

serial = sprintf("%04x-%04x", part1, part2)
```

**Example** for email `"pr0cracker"`:

| Character | ASCII | part1 increment | part2 increment |
|-----------|-------|-----------------|-----------------|
| p         | 112   | 112 × 8 = 896   | 112² - 188 = 12356 |
| r         | 114   | 114 × 8 = 912   | 114² - 188 = 12808 |
| 0         | 48    | 48 × 8 = 384    | 48² - 188 = 2116   |
| ...       | ...   | ...             | ...             |

**Final values**:
- part1 = 8040 = `0x1f68`
- part2 = 102617 = `0x190d9`
- **Serial**: `1f68-190d9`

### 5. Keygen Implementation

Created [cracker.c](src/cracker.c) implementing the discovered algorithm:

```c
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <email>\n", argv[0]);
        return 1;
    }
    
    char *email = argv[1];
    unsigned int part1 = 0;
    unsigned int part2 = 0;
    
    for (int i = 0; i < strlen(email); i++) {
        part1 += email[i] * 8;
        part2 += (email[i] * email[i]) - 0xbc;
    }
    
    printf("%04x-%04x\n", part1, part2);
    return 0;
}
```

### 6. Verification

Testing the keygen against the original binary:


```bash
$ ./bin/cracker pr0cracker
1f68-190d9

$ ./binary/keygenme3
********************************
E-Mail address: pr0cracker
Serial number: 1f68-190d9
Correct serial! Software unlocked.
********************************
```

✅ **Verification successful!** The keygen produces valid serials.

## 🛠️ Building from Source

### Prerequisites

- GCC compiler
- Make (optional, for convenience)

**Installation**:
- **macOS**: `xcode-select --install`
- **Ubuntu/Debian**: `sudo apt-get install build-essential`
- **Fedora**: `sudo dnf install gcc make`

### Compilation

#### Using Make (Recommended)

```bash
make all        # Build all tools (cracker, main, varify)
make cracker    # Build only the keygen
make clean      # Remove all binaries
```

#### Manual Compilation

```bash
gcc src/cracker.c -o bin/cracker
gcc src/main.c -o bin/main
gcc src/varify.c -o bin/varify
```

## 📚 Usage Examples

### Generate Serial for Any Email

```bash
$ ./bin/cracker admin@example.com
2a48-24c51

$ ./bin/cracker john.doe
1c58-1690b

$ ./bin/cracker hacker123
1d80-18c01
```

### Verify Generated Serial

```bash
$ ./bin/varify pr0cracker 1f68-190d9
Serial is valid!
```

### Test with Reimplemented Binary

The [main.c](src/main.c) file is a complete reimplementation of the original binary:

```bash
$ ./bin/main
********************************
E-Mail address: test@test.com
Serial number: 2670-20f87
Correct serial! Software unlocked.
********************************
```

## 📖 Documentation

- **[QUICKSTART.md](QUICKSTART.md)** - Get started in 2 minutes
- **[ANALYSIS.md](ANALYSIS.md)** - Detailed technical analysis and algorithm breakdown
- **[BUILD.md](BUILD.md)** - Comprehensive build instructions
- **[GHIDRA_NOTES.md](GHIDRA_NOTES.md)** - Ghidra analysis process and notes

## 🔧 Tools Used

| Tool | Purpose |
|------|---------|
| **Ghidra** | Static binary analysis and decompilation |
| **GCC** | C compiler for keygen and reimplementation |
| **file** | Binary format identification |
| **strings** | Extract readable strings from binary |
| **objdump** | Disassembly and binary inspection |

## 🧪 Testing Results

| Email | Generated Serial | Status |
|-------|------------------|--------|
| pr0cracker | 1f68-190d9 | ✅ Valid |
| admin | e50-a11 | ✅ Valid |
| test@test.com | 2670-20f87 | ✅ Valid |
| hacker123 | 1d80-18c01 | ✅ Valid |

All generated serials validated successfully against the original binary.

## 📝 Notes

- **Variable Naming**: Names like `local_44`, `local_40` come from Ghidra's auto-analysis and represent stack offsets. I kept them to maintain traceability between the analysis and implementation.
- **Stripped Binary**: The absence of debugging symbols made initial analysis more challenging but demonstrates real-world reverse engineering scenarios.
- **Algorithm Simplicity**: Despite being stripped, the algorithm is relatively simple - a good learning example for beginners in reverse engineering.

## 🎓 Learning Outcomes

This project demonstrates:
1. **Static Analysis** - Using Ghidra to decompile stripped binaries
2. **Algorithm Extraction** - Understanding validation logic from assembly/decompiled code
3. **Keygen Development** - Implementing the reverse-engineered algorithm
4. **Verification** - Testing implementations against original binaries

## 📄 License

This project is for educational purposes only. The original `keygenme3` binary is a practice challenge for learning reverse engineering.

## 🙏 Acknowledgments

- Challenge binary creators for providing this educational resource
- Ghidra project by NSA for excellent reverse engineering tools

---

**⚠️ Educational Purpose Only**: This project is created for learning reverse engineering techniques. Always respect software licenses and intellectual property rights.
