# Ghidra Analysis Notes

How I used Ghidra to reverse engineer keygenme3.

## Initial Setup

### Check the Binary

```bash
$ file keygenme3
keygenme3: ELF 64-bit LSB executable, x86-64, version 1 (SYSV),
dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2,
BuildID[sha1]=84fc4344dc69402219a6f4cbfe4c04fa00961ff1, 
for GNU/Linux 3.2.0, stripped
```

Key point: It's stripped. No function names or symbols.

### Open in Ghidra

1. Created new project
2. Imported keygenme3
3. Ran auto-analysis with default settings

## Function Discovery

### Finding the Functions

Since the binary was stripped, Ghidra gave generic names to everything.

I found two important functions:
- Entry point function (renamed to `main`)
- Validation function (renamed to `serial_check`)

![Main and serial_check functions](images/main_fun_ghidra.png)

## The serial_check Function

This is where the serial validation happens.

![serial_check detail](images/serial_chk_ghidra.png)

### Decompiled Code

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

## Understanding the Algorithm

### Variables

- `local_44` - Accumulator for (character * 8)
- `local_40` - Accumulator for (character² - 0xbc)
- `local_3c` - Loop counter
- `local_38` - Buffer for the generated serial

Note: These names come from Ghidra. They're based on stack offsets.

### The Loop

For each character in the email:
```c
local_44 = local_44 + (int)email[local_3c] * 8;
local_40 = (local_40 + (int)email[local_3c] * (int)email[local_3c]) - 0xbc;
```

### The Format String

```c
sprintf(local_38, "%04x-%04x", local_44, local_40);
```

This creates a serial like: `1f68-190d9`

### The Validation

```c
iVar1 = strcmp(serial, local_38);
return iVar1 == 0;
```

Just compares the input serial with the generated one.

## Creating the Crack

Once I understood the algorithm, I wrote cracker.c:

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

I kept the variable names from Ghidra to match the analysis.

## Testing

```bash
$ gcc cracker.c -o cracker
$ ./cracker pr0cracker
1f68-190d9

$ ./keygenme3
********************************
E-Mail address: pr0cracker
Serial number: 1f68-190d9
Correct serial! Software unlocked.
********************************
```

It works.

## Key Points

1. Use `file` command first to understand what you're dealing with
2. Stripped binaries need function renaming
3. Ghidra's decompiler makes finding algorithms much easier than reading assembly
4. Keep variable names from Ghidra for traceability
5. The format string (`%04x-%04x`) tells you exactly what the output looks like
6. Test your understanding by implementing it

## Magic Constant

The constant `0xbc` (188 in decimal) is subtracted in the second accumulator. Not sure why this specific value, but it's part of the algorithm.
