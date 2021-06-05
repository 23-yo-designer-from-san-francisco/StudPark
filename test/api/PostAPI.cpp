#include "gtest/gtest.h"

#include "../../src/api/PostAPI.cpp"

TEST(post, create_post) {
    ASSERT_EQ(1, 1);
}

TEST(post, delete_post) {
    ASSERT_EQ(1, 1);
}

TEST(post, comment_post) {
    ASSERT_EQ(1, 1);
}

TEST(post, delete_comment) {
    ASSERT_EQ(1, 1);
}

TEST(post, check_post_visibility) {  // Для потока или для группы
    ASSERT_EQ(1, 1);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
