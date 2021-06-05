#ifndef INCLUDE_ORM_GROUP_H_
#define INCLUDE_ORM_GROUP_H_

#include <ctime>
#include <string>
#include <vector>
#include "Wrapper.h"
#include "Student.h"

class Group {
 public:
    explicit Group(
        int id_group,
        const std::string &user_faculty,
        int user_number_departament,
        int user_semester,
        int user_group_number,
        const std::string &_education_level,
        const std::string &_join_code,
        time_t user_start_week) :
        id(id_group),
        faculty(user_faculty),
        number_departament(user_number_departament),
        semester(user_semester),
        group_number(user_group_number),
        education_level(_education_level),
        join_code(_join_code),
        start_week(user_start_week) {}
    static int AddGroup(SQLWrapper &db, const std::string &user_faculty, int user_number_departament, int user_semester,
                        int user_group_number, const std::string &education_level, const std::string &join_code, const std::string &start_week);
    static Group GetGroupById(SQLWrapper &db, int group_id);
    static std::vector<Student> GetMembers(SQLWrapper &db, int group_id);
    static int GetGroupByJoinCode(SQLWrapper &db, const std::string &join_code);
    static std::vector<Student> GetPostNotificationSubscribers(SQLWrapper &db, int id);
    static std::string GetGroupName(SQLWrapper &db, int id);

    int id;
    std::string faculty;
    int number_departament;
    int semester;
    int group_number;
    std::string education_level;
    std::string join_code;
    time_t start_week;
};

#endif  // INCLUDE_ORM_GROUP_H_
