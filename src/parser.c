#include "parser.h"
#include "interface.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024

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

            // Extract interface name
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

            // Extract status (e.g., UP, DOWN, etc.)
            char *status_start = strchr(line, '<');
            if (status_start) {
                char *status_end = strchr(status_start, '>');
                if (status_end) {
                    size_t len = status_end - status_start - 1;
                    strncpy(current.status, status_start + 1, len);
                    current.status[len] = '\0';
                }
            }

            // Extract MTU (if present)
            char *mtu_start = strstr(line, "mtu ");
            if (mtu_start) {
                sscanf(mtu_start, "mtu %d", &current.mtu);
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

