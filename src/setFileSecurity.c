#include <windows.h>
#include <stdio.h>

int main() {

    // Windows ownership change is complex, so we demonstrate API call usage
    BOOL result = SetFileSecurity(
        "ecg_output.txt",
        OWNER_SECURITY_INFORMATION,
        NULL   // simplified demo
    );

    if (result) {
        printf("Ownership function called (demo)\n");
    } else {
        printf("SetFileSecurity failed (expected without full setup)\n");
    }

    return 0;
}