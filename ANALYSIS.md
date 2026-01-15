# Technical Analysis

Detailed breakdown of the keygenme3 algorithm.

## Binary Information

```bash
$ file keygenme3
keygenme3: ELF 64-bit LSB executable, x86-64, version 1 (SYSV),
dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2,
BuildID[sha1]=84fc4344dc69402219a6f4cbfe4c04fa00961ff1, 
for GNU/Linux 3.2.0, stripped
```

- 64-bit ELF executable
- x86-64 architecture
- Stripped (no symbols)
- Dynamically linked

## The Algorithm

### serial_check Function

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

### Variables

| Variable | Type | Purpose |
|----------|------|---------|
| local_44 | uint | Sum of (character * 8) |
| local_40 | uint | Sum of (character² - 0xbc) |
| local_3c | int | Loop index |
| local_38 | char[24] | Generated serial string |

### Step-by-Step

1. Loop through each character in the email
2. Add `character * 8` to local_44
3. Add `character² - 0xbc` to local_40
4. Format as "%04x-%04x" (two 4-digit hex numbers with a dash)
5. Compare with the input serial

## Manual Calculation

For email "pr0cracker":

```
Characters: p(112) r(114) 0(48) c(99) r(114) a(97) c(99) k(107) e(101) r(114)

local_44 (character * 8):
  112*8 = 896
  114*8 = 912
  48*8 = 384
  99*8 = 792
  114*8 = 912
  97*8 = 776
  99*8 = 792
  107*8 = 856
  101*8 = 808
  114*8 = 912
  Total: 8040 = 0x1f68

local_40 (character² - 0xbc):
  112² - 188 = 12544 - 188 = 12356
  114² - 188 = 12996 - 188 = 12808
  48² - 188 = 2304 - 188 = 2116
  99² - 188 = 9801 - 188 = 9613
  114² - 188 = 12996 - 188 = 12808
  97² - 188 = 9409 - 188 = 9221
  99² - 188 = 9801 - 188 = 9613
  107² - 188 = 11449 - 188 = 11261
  101² - 188 = 10201 - 188 = 10013
  114² - 188 = 12996 - 188 = 12808
  Total: 102617 = 0x190d9

Serial: 1f68-190d9
```

## The Format String

```c
sprintf(local_38, "%04x-%04x", local_44, local_40);
```

- `%04x` - Hexadecimal, minimum 4 digits, zero-padded
- First value: local_44
- `-` - Literal dash
- Second value: local_40

Note: If the hex value is more than 4 digits, it still prints all digits. The `04` is just the minimum width.

## Implementation

The crack script uses the same logic:

```c
unsigned int local_44 = 0;
unsigned int local_40 = 0;

for (int i = 0; i < strlen(email); i++) {
    local_44 += email[i] * 8;
    local_40 += (email[i] * email[i]) - 0xbc;
}

printf("%04x-%04x\n", local_44, local_40);
```

## The Magic Constant

`0xbc` = 188 in decimal

This is subtracted from each character's square. The choice of 188 seems arbitrary - might be to prevent overflow or just to make the algorithm more complex.

## Security

This is a weak algorithm:
- Deterministic (same email always gives same serial)
- No cryptography
- Easy to reverse engineer
- No server-side validation

A better approach would use:
- Cryptographic signatures
- Time-based components
- Server validation
- Public/private key pairs
