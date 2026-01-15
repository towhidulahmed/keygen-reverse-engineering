#include <stdio.h>
#include <string.h>

int main(void) {
    char *email = "pr0cracker";
    char serial[32];
    unsigned int local_44 = 0;
    unsigned int local_40 = 0;
    int local_3c = 0;

    // Generating serial number for the user pr0cracker
    while (local_3c < strlen(email)) {
        local_44 = local_44 + email[local_3c] * 8;
        local_40 = (local_40 + (int)email[local_3c] * (int)email[local_3c]) - 0xbc;
        local_3c = local_3c + 1;
    }
    sprintf(serial, "%04x-%04x", local_44, local_40);

    printf("Email: %s\n", email);
    printf("Generated serial: %s\n", serial);

    return 0;
}