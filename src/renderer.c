
#include "renderer.h"
#include "interface.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define COLOR_RESET "\033[0m"
#define COLOR_BOLD_GREEN "\033[1;32m"

static void print_border(int widths[], int n, const char* left, const char* mid, const char* right, const char* fill) {
    printf("%s", left);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < widths[i] + 2; ++j)
            printf("%s", fill);
        printf("%s", (i == n - 1) ? right : mid);
    }
    putchar('\n');
}

static void print_row(char *cells[], int widths[], int n, bool bold) {
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

void render_table(void) {
    int width_num = 2, width_name = 4, width_mac = 3, width_ipv4 = 4, width_ipv6 = 4, width_status = 5, width_mtu = 4;

    for (int i = 0; i < iface_count; ++i) {
        char idx[4];
        snprintf(idx, sizeof(idx), "%d", i);
        width_num = max(width_num, (int)strlen(idx));
        width_name = max(width_name, (int)strlen(interfaces[i].name));
        width_mac  = max(width_mac, (int)strlen(interfaces[i].mac));
        width_ipv4 = max(width_ipv4, (int)strlen(interfaces[i].ipv4));
        width_ipv6 = max(width_ipv6, (int)strlen(interfaces[i].ipv6));
        width_status = max(width_status, (int)strlen(interfaces[i].status));

        // Use snprintf to format mtu as a string and calculate its length
        char mtu_str[16];  // Large enough to hold the mtu value as a string
        snprintf(mtu_str, sizeof(mtu_str), "%d", interfaces[i].mtu);
        width_mtu = max(width_mtu, (int)strlen(mtu_str));
    }

    // Calculate the column widths dynamically
    int widths[] = {width_num, width_name, width_mac, width_ipv4, width_ipv6, width_status, width_mtu};
    const int cols = sizeof(widths) / sizeof(widths[0]);

    // Top border
    print_border(widths, cols, "╭", "┬", "╮", "─");

    // Header row
    char *headers[] = {"#", "Name", "MAC", "IPv4", "IPv6", "Status", "MTU"};
    print_row(headers, widths, cols, true);

    // Mid border
    print_border(widths, cols, "├", "┼", "┤", "─");

    // Data rows
    for (int i = 0; i < iface_count; ++i) {
        char idx[4];
        snprintf(idx, sizeof(idx), "%d", i);
        
        // Use snprintf to convert mtu to a string for display
        char mtu_str[16];
        snprintf(mtu_str, sizeof(mtu_str), "%d", interfaces[i].mtu);

        char *cells[] = {
            idx,
            interfaces[i].name,
            interfaces[i].mac[0] ? interfaces[i].mac : "-",
            interfaces[i].ipv4[0] ? interfaces[i].ipv4 : "-",
            interfaces[i].ipv6[0] ? interfaces[i].ipv6 : "-",
            interfaces[i].status[0] ? interfaces[i].status : "-",
            mtu_str
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
