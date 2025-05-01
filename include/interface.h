#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdbool.h> // Add this line

#define MAX_INTERFACES 256

typedef struct {
    char name[64];
    char mac[32];
    char ipv4[64];
    char ipv6[128];
    char status[64];
    int mtu;
    bool is_current_device; // Add this line
} Interface;

extern Interface interfaces[MAX_INTERFACES];
extern int iface_count;

void parse_ip_output(FILE *fp);
void render_table(void);

#endif // INTERFACE_H
