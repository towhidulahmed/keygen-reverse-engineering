#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// read_line implementation
char *read_line(void) {
    char *s;
    size_t len;

    s = (char *)malloc(0x32);        // allocate 50 bytes
    fgets(s, 0x31, stdin);           // read up to 49 chars
    len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';           // strip newline
    }
    return s;
}

// validate_serial implementation
bool validate_serial(char *email, char *serial) {
    unsigned int local_44 = 0;
    unsigned int local_40 = 0;
    int local_3c = 0;
    char local_38[24];

    while (1) {
        size_t sVar2 = strlen(email);
        if (sVar2 <= (unsigned long)local_3c) break;

        local_44 = local_44 + email[local_3c] * 8;
        local_40 = (local_40 + (int)email[local_3c] * (int)email[local_3c]) - 0xbc;
        local_3c = local_3c + 1;
    }

    sprintf(local_38, "%04x-%04x", local_44, local_40);

    int iVar1 = strcmp(serial, local_38);
    return iVar1 == 0;
}

// main
int main(void) {
    printf("********************************\n");
    printf("E-Mail address: ");
    char *email = read_line();

    printf("Serial number: ");
    char *serial = read_line();

    if (validate_serial(email, serial)) {
        puts("Correct serial! Software unlocked.");
        printf("********************************\n");
        printf("_____/\\__/\\_|  -------  |_/_____\n");
    } else {
        puts("Incorrect serial.");
        printf("********************************\n");
        printf("_____/\\_/\\_/\\_|  -------  |_\\___\\\n");
    }

    free(email);
    free(serial);
    return 0;
}
