#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_SIZE 100

static char buffer[MAX_COMMAND_SIZE];

// Send commands to the server
const char* move_command(uint32_t ship_id, uint16_t angle, uint16_t speed) {
    sprintf(buffer, "MOVE %d %d %d", ship_id, angle, speed);
    return buffer;
}

const char* fire_command(uint8_t ship_id, uint16_t angle) {
    sprintf(buffer, "FIRE %d %d", ship_id, angle);
    return buffer;
}

const char* radar_command(uint8_t ship_id) {
    sprintf(buffer, "RADAR %d", ship_id);
    return buffer;
}

// Split a string uinto an array of strings using strtok
char** split(const char* str, const char delimiter) {
    char delimiter_str[2];
    delimiter_str[0] = delimiter;
    delimiter_str[1] = '\0';
    uint16_t count;
    uint16_t i = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == delimiter) {
            count++;
        }
    }
    count++;

    char* token;
    char** tokens = malloc(sizeof(char*) * count);
    if (!tokens) return NULL;
    i = 0;
    char* tmp = strdup(str);
    token = strtok(tmp, delimiter_str);
    while (token != NULL) {
        tokens[i] = token;
        token = strtok(NULL, delimiter_str);
        i++;
    }
    return tokens;
}
// Parse responses from the server
int parse_radar_response(const char* response) {
    char **str_scan = split(response, ',');
    // don't forget to free the memory of str_scan
    return 0;
}