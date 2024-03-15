#include "minunit.h"
#include "functionCalculs.h"

MU_TEST(test_calcul_angle)
{
    mu_assert_double_eq(90, getTravelAngle(15, 10, 15, 5));
    mu_assert_double_eq(315, getTravelAngle(25, 13, 27, 15));
    mu_assert_double_eq(135, getTravelAngle(100, 80, 80, 60));
}

MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(test_calcul_angle);
}

int main(void)
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}
