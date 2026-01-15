#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validate_serial(char *email, char *serial) {
    unsigned int local_44 = 0;
    unsigned int local_40 = 0;
    int local_3c = 0;
    char local_38[24];

    while (local_3c < strlen(email)) {
        local_44 = local_44 + email[local_3c] * 8;
        local_40 = (local_40 + (int)email[local_3c] * (int)email[local_3c]) - 0xbc;
        local_3c = local_3c + 1;
    }

    //sprintf(local_38, "%04x-%04x", local_44, local_40);

    return strcmp(serial, local_38) == 0;
}

int main(void) {
    char *email = "pr0cracker";
    char serial[32];
    unsigned int local_44 = 0;
    unsigned int local_40 = 0;
    int local_3c = 0;

    // Generating correct serial for given email
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

//Email: pr0cracker
//serial: 1f68-190d9