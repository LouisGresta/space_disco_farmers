#include <stdio.h>

#define MAX_COMMAND_SIZE 100

static char buffer[MAX_COMMAND_SIZE];

const char* move_command(int ship_id, int angle, int speed) {
    sprintf(buffer, MAX_COMMAND_SIZE, "MOVE %d %d %d\n", ship_id, angle, speed);
    return buffer;
}

const char* fire_command(int ship_id, int angle) {
    sprintf(buffer, MAX_COMMAND_SIZE, "FIRE %d %d\n", ship_id, angle);
    return buffer;
}

const char* radar_command(int ship_id) {
    sprintf(buffer, MAX_COMMAND_SIZE, "RADAR %d\n", ship_id);
    return buffer;
}
