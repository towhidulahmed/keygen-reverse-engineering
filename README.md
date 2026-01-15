# KeygenMe3 Reverse Engineering Challenge

Reverse engineering challenge to understand the serial validation mechanism of a stripped x86-64 ELF binary and generate valid serial numbers for any given email address.

## Challenge Description

The binary `keygenme3` prompts for an email address and a serial number. The objective was to reverse engineer the validation logic and recover the correct serial for a given user.

![KeygenMe3 Running](images/run_keygenme3.png)

## Analysis Method

### Initial Binary Inspection

The binary was first inspected using the `file` command:
```bash
file binary/keygenme3
```

```
keygenme3: ELF 64-bit LSB executable, x86-64, version 1 (SYSV),
dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2,
for GNU/Linux 3.2.0, stripped
```

This revealed that the file is an ELF 64-bit LSB executable for the x86-64 architecture, and importantly, it is **stripped** (no debugging symbols or function names).

### Ghidra Analysis

The binary was opened in Ghidra for decompilation and analysis. During the analysis, several functions and variable names were renamed for clarity. Two important functions were identified and renamed as `main` and `serial_check`.

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

## Algorithm Implementation

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

To verify the implementation, compile and run the keygen:

```bash
gcc src/cracker.c -o bin/cracker
./bin/cracker pr0cracker
```

Output:
```
1f68-190d9
```

The generated serial `1f68-190d9` was then tested with the original binary:

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

The program successfully validated the input and displayed the success message.

## Conclusion

The binary validates user input by deriving a serial number from the email address. By reversing the function logic in Ghidra and reimplementing it in C, the correct serial for any user can be generated.

**Example for user "pr0cracker"**: `1f68-190d9`

## Repository Structure

```
keygen-reverse-engineering/
├── binary/keygenme3    # Original challenge binary
├── src/cracker.c       # Keygen implementation
├── images/             # Analysis screenshots
└── Makefile           # Build configuration
```

## Building

Compile the keygen using:

```bash
gcc src/cracker.c -o bin/cracker
```

Or use the provided Makefile:

```bash
make cracker
```

## Tools Used

- **Ghidra** - Binary decompilation and analysis
- **GCC** - C compiler

## Notes

Variable names like `local_44`, `local_40`, and `local_3c` are Ghidra's auto-generated names based on stack offsets. They were kept in the implementation to maintain traceability with the decompiled code.

## License

This project is for educational purposes only.
