#include <gtest/gtest.h>

extern "C" {
}

TEST(BasicAssertion, Equality) {
    ASSERT_EQ(2+2, 4);
}
