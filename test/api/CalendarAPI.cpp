#include "gtest/gtest.h"
#include "../../src/api/CalendarAPI.cpp"

TEST(calendar, add_event) {  // Доделать
    int user = User::AddUser();  // Create test user
    int subject = Subject::AddSubject()     // Create test subject
    int deadline = DeadLines::AddDeadline(user, subject, date);                             // Add test deadline
    // DeadLines::GetDeadline                             // Check if test deadline exists
                                 // Delete everything
}

TEST(calendar, edit_event) {
}

TEST(calendar, delete_event) {
}

TEST(calendar, get_events_for_user) {
}

TEST(calendar, export_ics) {
}

TEST(calendar, add_deadline) {
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
