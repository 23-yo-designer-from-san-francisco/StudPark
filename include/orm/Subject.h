#ifndef INCLUDE_ORM_SUBJECT_H_
#define INCLUDE_ORM_SUBJECT_H_

#include <iostream>
#include <vector>
#include <string>
#include "Wrapper.h"

class Subject {
 public:
    explicit Subject(
            int id_subject,
            int _group_id,
            const std::string &_subject) :
            id(id_subject),
            subject(_subject),
            group_id(_group_id) {}
    Subject() = default;
    static int AddSubject(SQLWrapper &db, const std::string &subject, int group_id);
    static void DeleteSubject(SQLWrapper &db, int subject_id);
    static Subject GetSubjectById(SQLWrapper &db, int subject_id);
    static std::vector<Subject> ListSubject(SQLWrapper &db, int group_id);

    int id;
    int group_id;
    std::string subject;
};

#endif  // INCLUDE_ORM_SUBJECT_H_
