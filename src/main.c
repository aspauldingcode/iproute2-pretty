#include "interface.h"
#include "parser.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *fp = popen("ip a", "r");
    if (!fp) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    parse_ip_output(fp);
    pclose(fp);

    render_table();
    return 0;
}
