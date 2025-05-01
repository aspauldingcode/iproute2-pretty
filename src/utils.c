#include "utils.h"
#include <string.h>

void trim_newline(char *str) {
    char *pos;
    if ((pos = strchr(str, '\n')) != NULL)
        *pos = '\0';
}

int max(int a, int b) {
    return a > b ? a : b;
}
