#include "Wrapper.h"
#include "Attendance.h"
#include "utils_wrapper.hpp"

int Attendance::AddAttendance(SQLWrapper &db, int user_id, int subject_id, time_t date_pass) {
    db << "INSERT INTO attendance (user_fk, subject_fk, date_pass) values ('"
       << user_id << "', "
       << subject_id << ", to_timestamp("
       << date_pass << ")) returning id;";
    db.exec();

    return db.get_int(0);
}

void Attendance::DeleteAttendance(SQLWrapper &db, int Attendance_id) {
    db << "DELETE FROM attendance WHERE id = " << Attendance_id << ";";
    db.exec();
}

Attendance Attendance::GetAttendance(SQLWrapper &db, int user_id, int subject_id) {
    if (check_existence(db, "attendance", "user_fk", user_id)) {
        throw std::length_error("ERROR: FIELD attendance.user_fk NOT FOUND");
    }
    if (check_existence(db, "attendance", "subject_fk", subject_id)) {
        throw std::length_error("ERROR: FIELD attendance.subject_fk NOT FOUND");
    }
    db << "SELECT * FROM attendance WHERE user_fk = " << user_id << " AND subject_fk = " << subject_id << ";";
    db.exec();
    Attendance result(
                   db.get_int(0),
                   db.get_int(1),
                   db.get_int(2),
                   db.get_time_t(3));
    return result;
}
