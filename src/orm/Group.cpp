#include "Group.h"
#include "Wrapper.h"
#include "utils_wrapper.hpp"
#include "Student.h"

int Group::AddGroup(SQLWrapper &db, const std::string &user_faculty, int user_number_departament, int user_semester,
                    int user_group_number, const std::string &education_level, const std::string &join_code, const std::string &start_week) {
    db << "INSERT INTO \"group\" (faculty, number_departament, semester,"
          " group_number, education_level, start_week, join_code) values ('"
       << user_faculty << "', "
       << user_number_departament << ", "
       << user_semester << ", "
       << user_group_number << ", '"
       << education_level << "', '"<< start_week << "-09-01 00:00:00', '"
       << join_code << "') returning id;";
    db.exec();

    return db.get_int(0);
}

Group Group::GetGroupById(SQLWrapper &db, int group_id) {
    if (check_existence(db, "\"group\"", "id", group_id)) {
        throw std::length_error("ERROR: FIELD group.id NOT FOUND");
    }

    db << "SELECT * FROM \"group\" WHERE id = '" << group_id << "';";
    db.exec();
    Group result(
            db.get_int(0),
            db.get_str(1),
            db.get_int(2),
            db.get_int(3),
            db.get_int(4),
            db.get_str(5),
            db.get_str(6),
            db.get_time_t(7));
    return result;
}

std::vector<Student> Group::GetMembers(SQLWrapper &db, int group_id) {
    db << "SELECT * FROM student WHERE group_id = '" << group_id << "' ORDER BY second_name;";
    db.exec();
    int i = 0;
    std::vector<Student> result;
    while (db.count_tuples() > i) {
        Student request(db.get_str(0, i),
                        db.get_str(1, i),
                        db.get_str(2, i),
                        db.get_bool(3, i),
                        db.get_str(4, i),
                        db.get_str(5, i),
                        db.get_str(6, i),
                        db.get_int(7, i),
                        db.get_int(8, i),
                        db.get_str(9, i),
                        db.get_str(10, i),
                        db.get_bool(11, i),
                        db.get_int(12, i),
                        db.get_int(13, i),
                        db.get_time_t(14, i),
                        db.get_int(15, i),
                        db.get_bool(16, i),
                        db.get_str(17, i));
        result.push_back(request);
        i++;
    }
    return result;
}

int Group::GetGroupByJoinCode(SQLWrapper &db, const std::string &join_code) {
    db << "SELECT id FROM \"group\" WHERE join_code = '" << join_code << "';";
    db.exec();
    return db.get_int(0);
}

std::vector<Student> Group::GetPostNotificationSubscribers(SQLWrapper &db, int id) {
    db << "SELECT * FROM student WHERE group_id = '" << id << "' AND notification = 'true';";
    db.exec();
    int i = 0;
    std::vector<Student> result;
    while (db.count_tuples() > i) {
        Student request(db.get_str(0, i),
                        db.get_str(1, i),
                        db.get_str(2, i),
                        db.get_bool(3, i),
                        db.get_str(4, i),
                        db.get_str(5, i),
                        db.get_str(6, i),
                        db.get_int(7, i),
                        db.get_int(8, i),
                        db.get_str(9, i),
                        db.get_str(10, i),
                        db.get_bool(11, i),
                        db.get_int(12, i),
                        db.get_int(13, i),
                        db.get_time_t(14, i),
                        db.get_int(15, i),
                        db.get_bool(16, i),
                        db.get_str(17, i));
        result.push_back(request);
        i++;
    }
    return result;
}

std::string Group::GetGroupName(SQLWrapper &db, int id) {
    if (check_existence(db, "\"group\"", "id", id)) {
        throw std::length_error("ERROR: FIELD group.id NOT FOUND");
    }
    db << "SELECT faculty, number_departament, semester, group_number, education_level FROM \"group\" WHERE id = '" << id << "';";
    db.exec();
    std::string result = std::string(db.get_str(0)) + db.get_str(1) + "-" + db.get_str(2) + db.get_str(3) + db.get_str(4);    return result;
}
