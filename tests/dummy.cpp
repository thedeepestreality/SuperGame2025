#include <gtest/gtest.h>

TEST(Dummy, echo_test)
{
    int x = 1;
    int y = 1;
    EXPECT_EQ(y, x);
}
