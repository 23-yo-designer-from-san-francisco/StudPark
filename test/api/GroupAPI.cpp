#include "gtest/gtest.h"

#include "../../src/api/GroupAPI.cpp"

TEST(group, add_group) {
ASSERT_EQ(1, 1);
}

TEST(group, add_group_member) {
ASSERT_EQ(1, 1);
}

TEST(group, delete_group_member) {
ASSERT_EQ(1, 1);
}

TEST(group, export_group_list) {
ASSERT_EQ(1, 1);
}

TEST(group, add_journal_entry) {
ASSERT_EQ(1, 1);
}

TEST(group, create_lab_queue) {
ASSERT_EQ(1, 1);
}

TEST(group, swap_lab_queue) {
ASSERT_EQ(1, 1);
}

TEST(group, modify_lab_queue) {
ASSERT_EQ(1, 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
