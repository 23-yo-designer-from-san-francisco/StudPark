#include "gtest/gtest.h"
#include "../../src/api/UserAPI.cpp"

TEST(user, create_user) {
    ASSERT_EQ(1, 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
