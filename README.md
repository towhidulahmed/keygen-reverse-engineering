# KeygenMe3 Reverse Engineering Challenge

Reverse engineering challenge to understand the serial validation mechanism of a stripped x86-64 ELF binary and generate valid serial numbers for any given email address.

## Challenge Description

The binary `keygenme3` prompts for an email address and a serial number. The objective was to reverse engineer the validation logic and recover the correct serial for a given user.

## Analysis Method

### Initial Binary Inspection

The binary was first inspected using the `file` command:

```bash
file challenge/keygenme3
```

```
keygenme3: ELF 64-bit LSB executable, x86-64, version 1 (SYSV),
dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2,
BuildID[sha1]=84fc4344dc69402219a6f4cbfe4c04fa00961ff1, 
for GNU/Linux 3.2.0, stripped
```

This revealed that the file is an ELF 64-bit LSB executable for the x86-64 architecture, and importantly, it is **stripped** (no debugging symbols or function names).

### Ghidra Analysis

The binary was opened in Ghidra for decompilation and analysis. Since the binary was stripped, Ghidra assigned generic names to all functions. After auto-analysis, two important functions were identified and renamed:
- Entry point function → `main`
- Validation function → `serial_check`

![Ghidra Main Function Analysis](images/main_fun_ghidra.png)

### Serial Validation Logic

The critical function `serial_check` contains the validation logic:

![Serial Check Function in Ghidra](images/serial_chk_ghidra.png)

Inside `serial_check`, the algorithm performs the following steps for each character of the email:

1. **`local_44`** accumulates each character multiplied by 8
2. **`local_40`** accumulates the square of each character, with a constant subtraction of `0xbc` (188 decimal) applied at each step

After the loop completes, the two results (`local_44` and `local_40`) are formatted into a serial string using:

```c
sprintf(local_38, "%04x-%04x", local_44, local_40);
```

The generated string is then compared against the provided serial number. If they match, the program outputs a success message.

### Algorithm Breakdown

The decompiled `serial_check` function:

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

**Variable purposes**:
- `local_44` - Sum of (character × 8)
- `local_40` - Sum of (character² - 0xbc)
- `local_3c` - Loop index
- `local_38` - Generated serial string buffer

### Manual Calculation Example

For email "pr0cracker":

**Characters**: p(112), r(114), 0(48), c(99), r(114), a(97), c(99), k(107), e(101), r(114)

**local_44 calculation** (character × 8):
```
112×8 + 114×8 + 48×8 + 99×8 + 114×8 + 97×8 + 99×8 + 107×8 + 101×8 + 114×8
= 896 + 912 + 384 + 792 + 912 + 776 + 792 + 856 + 808 + 912
= 8040 = 0x1f68
```

**local_40 calculation** (character² - 188):
```
(112²-188) + (114²-188) + (48²-188) + (99²-188) + (114²-188) + 
(97²-188) + (99²-188) + (107²-188) + (101²-188) + (114²-188)
= 12356 + 12808 + 2116 + 9613 + 12808 + 9221 + 9613 + 11261 + 10013 + 12808
= 102617 = 0x190d9
```

**Result**: Serial = `1f68-190d9`

## Keygen Implementation

To replicate this logic, a C program was written that implements the same calculation:

```c
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <email>\n", argv[0]);
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

## Testing

Compile the keygen:

```bash
gcc src/cracker.c -o keygen/cracker
```

Generate a serial for the user "pr0cracker":

```bash
./keygen/cracker pr0cracker
```

Output:
```
1f68-190d9
```

Verify with the original binary:

```bash
./challenge/keygenme3
```

```
********************************
E-Mail address: pr0cracker
Serial number: 1f68-190d9
Correct serial! Software unlocked.
********************************
```

![KeygenMe3 Running](images/run_keygenme3.png)

The program successfully validated the input and displayed the success message.

## Conclusion

The binary validates user input by deriving a serial number from the email address. By reversing the function logic in Ghidra and reimplementing it in C, the correct serial for any user can be generated.

**Example for user "pr0cracker"**: `1f68-190d9`

## Repository Structure

```
keygen-reverse-engineering/
├── challenge/
│   └── keygenme3       # Original challenge binary
├── src/
│   ├── cracker.c       # Keygen implementation
│   ├── main.c          # Reimplementation of keygenme3
│   └── varify.c        # Serial verification tool
├── keygen/             # Compiled binaries
└── images/             # Ghidra analysis screenshots
```

## Tools Used

- **Ghidra** - Binary decompilation and analysis
- **GCC** - C compiler

## Notes

Variable names like `local_44`, `local_40`, and `local_3c` are Ghidra's auto-generated names based on stack offsets. They were kept in the implementation to maintain traceability with the decompiled code.

## License

This project is for educational purposes only.
