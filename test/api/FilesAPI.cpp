#include "gtest/gtest.h"
#include "../../src/api/FilesAPI.cpp"

TEST(files, add_file) {
    // add a file to user's folder
    // check if file exists
}

TEST(files, share_file) {
    // add a file to user's folder
    // share a file to another user
    // check if another user can access this file
}

TEST(files, delete_file) {
}

TEST(files, list_user_files) {
    // Add files to user's folder,
    // List files,
    // Check if listed files are the same files that were added
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
