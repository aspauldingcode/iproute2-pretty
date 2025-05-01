#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_INTERFACES 256
#define MAX_LINE_LENGTH 1024
#define COLOR_RESET "\033[0m"
#define COLOR_BOLD_GREEN "\033[1;32m"

typedef struct {
    char name[64];
    char mac[32];
    char ipv4[64];
    char ipv6[128];
} Interface;

Interface interfaces[MAX_INTERFACES];
int iface_count = 0;

int max(int a, int b) { return a > b ? a : b; }

void trim_newline(char *str) {
    char *pos;
    if ((pos = strchr(str, '\n')) != NULL)
        *pos = '\0';
}

void parse_ip_output(FILE *fp) {
    char line[MAX_LINE_LENGTH];
    Interface current = {0};
    bool iface_open = false;

    while (fgets(line, sizeof(line), fp)) {
        trim_newline(line);

        if (line[0] >= '0' && line[0] <= '9') {
            if (iface_open) {
                interfaces[iface_count++] = current;
                memset(&current, 0, sizeof(Interface));
            }
            iface_open = true;

            char *name_start = strchr(line, ':');
            if (name_start && *(name_start + 1) == ' ') {
                name_start += 2;
                char *name_end = strchr(name_start, ':');
                if (name_end) {
                    size_t len = name_end - name_start;
                    strncpy(current.name, name_start, len);
                    current.name[len] = '\0';
                }
            }
        } else if (strstr(line, "link/ether")) {
            sscanf(line, "    link/ether %31s", current.mac);
        } else if (strstr(line, "inet ")) {
            sscanf(line, "    inet %63s", current.ipv4);
            char *slash = strchr(current.ipv4, '/');
            if (slash) *slash = '\0';
        } else if (strstr(line, "inet6 ")) {
            if (current.ipv6[0] == '\0') {
                sscanf(line, "    inet6 %127s", current.ipv6);
                char *slash = strchr(current.ipv6, '/');
                if (slash) *slash = '\0';
            }
        }
    }

    if (iface_open) {
        interfaces[iface_count++] = current;
    }
}

void print_border(int widths[], int n, const char* left, const char* mid, const char* right, const char* fill) {
    printf("%s", left);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < widths[i] + 2; ++j)
            printf("%s", fill);
        printf("%s", (i == n - 1) ? right : mid);
    }
    putchar('\n');
}

void print_row(char *cells[], int widths[], int n, bool bold) {
    printf("│");
    for (int i = 0; i < n; ++i) {
        if (i == 0) { // For the numbers column
            // Apply bold and base0B color (green)
            printf("\033[1;32m");
        } else if (bold) {
            // For other columns with bold style
            printf("\033[1;32m");
        }
        
        printf(" %-*s ", widths[i], cells[i]);
        
        if (i == 0) {
            printf("\033[0m"); // Reset color for the numbers column
        } else if (bold) {
            printf("\033[0m"); // Reset bold for other columns
        }
        
        printf("│");
    }
    putchar('\n');
}

void render_table() {
    int width_num = 2, width_name = 4, width_mac = 3, width_ipv4 = 4, width_ipv6 = 4;

    for (int i = 0; i < iface_count; ++i) {
        char idx[4];
        snprintf(idx, sizeof(idx), "%d", i);
        width_num = max(width_num, (int)strlen(idx));
        width_name = max(width_name, (int)strlen(interfaces[i].name));
        width_mac  = max(width_mac, (int)strlen(interfaces[i].mac));
        width_ipv4 = max(width_ipv4, (int)strlen(interfaces[i].ipv4));
        width_ipv6 = max(width_ipv6, (int)strlen(interfaces[i].ipv6));
    }

    // Calculate the column widths dynamically
    int widths[] = {width_num, width_name, width_mac, width_ipv4, width_ipv6};
    const int cols = sizeof(widths) / sizeof(widths[0]);

    // Top border
    print_border(widths, cols, "╭", "┬", "╮", "─");

    // Header row
    char *headers[] = {"#", "Name", "MAC", "IPv4", "IPv6"};
    print_row(headers, widths, cols, true);

    // Mid border
    print_border(widths, cols, "├", "┼", "┤", "─");

    // Data rows
    for (int i = 0; i < iface_count; ++i) {
        char idx[4];
        snprintf(idx, sizeof(idx), "%d", i);
        char *cells[] = {
            idx,
            interfaces[i].name,
            interfaces[i].mac[0] ? interfaces[i].mac : "-",
            interfaces[i].ipv4[0] ? interfaces[i].ipv4 : "-",
            interfaces[i].ipv6[0] ? interfaces[i].ipv6 : "-"
        };
        print_row(cells, widths, cols, false);
    }

    // Bottom border
    print_border(widths, cols, "╰", "┴", "╯", "─");

    // Footer row (if needed)
    if (iface_count >= 24) {
        print_row(headers, widths, cols, true);
    }
}

int main() {
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
