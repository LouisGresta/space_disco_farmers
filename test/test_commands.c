#include "minunit.h"
#include "../src/commands.c"

MU_TEST(test_move_command) {
    mu_assert_string_eq("MOVE 1 90 1000", move_command(1, 90, 1000));
    mu_assert_string_eq("MOVE 6 180 2000", move_command(6, 180, 2000));
    mu_assert_string_eq("MOVE 8 270 3000", move_command(8, 270, 3000));
}

MU_TEST(test_fire_command) {
    mu_assert_string_eq("FIRE 1 90", fire_command(1, 90));
    mu_assert_string_eq("FIRE 3 180", fire_command(3, 180));
    mu_assert_string_eq("FIRE 5 270", fire_command(5, 270));
}

MU_TEST(test_radar_command) {
    mu_assert_string_eq("RADAR 6", radar_command(6));
    mu_assert_string_eq("RADAR 7", radar_command(7));
}

MU_TEST(test_split) {
    uint16_t count;
    char** tokens = split("Theo,Louis,Pachelle Carelle", ',', &count);
    mu_assert_int_eq(3, count);
    mu_assert_string_eq("Theo", tokens[0]);
    mu_assert_string_eq("Louis", tokens[1]);
    mu_assert_string_eq("Pachelle Carelle", tokens[2]);
    free(tokens);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_move_command);
    MU_RUN_TEST(test_fire_command);
    MU_RUN_TEST(test_radar_command);
    MU_RUN_TEST(test_split);
}

int main(void) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}