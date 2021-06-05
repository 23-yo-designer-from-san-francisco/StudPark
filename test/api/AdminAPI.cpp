#include "gtest/gtest.h"
#include "../../src/api/AdminAPI.cpp"

TEST(admin_api, create_user) {
    // Insert user with admin role into test DB
    // Check if it actually exists
    // Delete test admin user
}

TEST(admin_api, create_group) {
    // Create test group, 
    // add a student,
    // check if a student belongs to the group
    // delete everything
}

TEST(admin_api, delete_user) {
    // create test user
    // check if the user is in the db
    // delete test user
    // check if we can't get the user by id
}

TEST(admin_api, delete_group) {
    // create test group
    // check if the group is in the db
    // delete test group
    // check if we can't get the group by id
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
