#include <Deadline.h>
#include "GroupAPI.h"
#include "Utils.h"

std::string GroupAPI::CreatePage(const std::unordered_map<std::string, std::string> &data) {
    Student admin;
    try {
        admin = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }
    if (admin.role != Student::Roles::ADMIN) {
        return templates.RenderErrors("Недостаточно прав", 1, admin.first_name + " " + admin.second_name);
    }
    return templates.RenderAdminPage(true);
}

std::string GroupAPI::Create(const std::unordered_map<std::string, std::string> &data) {
    Student admin;
    try {
        admin = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }
    std::cout << admin.role << std::endl;
    if (admin.role != Student::Roles::ADMIN) {
        return templates.RenderErrors("Недостаточно прав", 1, admin.first_name + " " + admin.second_name);
    }
    std::string joinCode = randomString(8);
    std::string faculty = data.find("user_faculty")->second;
    boost::locale::generator gen;
    std::locale loc = gen("");
    std::locale::global(loc);
    faculty = boost::locale::to_upper(faculty);
    std::cout << "FAK" << faculty << std::endl;
    int group = std::stoi(data.find("group")->second);
    try {
        group = Group::AddGroup(db,
                                faculty,
                                std::stoi(data.find("user_number_department")->second),
                                (group % 100) / 10,
                                group % 10,
                                data.find("education_level")->second,
                                joinCode,
                                data.find("start")->second);
    } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
    }

    std::string password = randomString(10);
    try {
        Student::AddStudentRegistration(db, data.find("head_f_name")->second, data.find("head_s_name")->second,
                                  data.find("head_patronymic")->second, data.find("head_user_login")->second,
                                  sha256(password), group, data.find("head_birthday")->second, Student::Roles::LEADER);
    } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
    }

    return templates.RenderGroupSuccessfulCreate("Пароль старосты: " + password + "\nКод присоединения: " + joinCode, 1, admin.first_name + " "  + admin.second_name);  // Render template "Group created, head password is: <pwd>
}
std::string GroupAPI::Get(const std::unordered_map<std::string, std::string> &data) {
    Student admin;
    try {
        admin = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return templates.RenderErrors(e.what());
    }

    return templates.RenderGroupList(db, admin.group_id, 1, admin.FullName(), admin.role);
}
std::string GroupAPI::Update(const std::unordered_map<std::string, std::string> &data) {
    return "";  // Delete
}
std::string GroupAPI::Delete(const std::unordered_map<std::string, std::string> &data) {
    return "";  // For superuser only
}

std::string GroupAPI::ExportGroupList(const std::unordered_map<std::string, std::string> &data) {
    Student admin;
    try {
        admin = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return e.what();
    }
    if (admin.role == Student::Roles::STUDENT) {
        return "Not enough privileges";
    }
    std::cout << admin.group_id << std::endl;

    std::vector<Student> members = Group::GetMembers(db, admin.group_id);

    std::string csv;
    csv += "№,ФИО,Email\n";
    size_t counter = 1;
    for (auto &s : members) {
        if (s.role == Student::Roles::LEADER) {
            csv += '*';
        }
        csv += std::to_string(counter++) + "," + s.second_name + " " + s.first_name + " " + s.patronymic + "," + s.login + "\n";
    }

    return csv;
}

std::string GroupAPI::AddSubjectPage(const std::unordered_map<std::string, std::string> &data) {
    std::string session = data.find("session")->second;
    Student admin = getStudentBySession(session);
    if (admin.role == Student::Roles::STUDENT) {
        return templates.RenderErrors("Not enough privileges");
    }
    std::cout << "ID: " << admin.first_name << std::endl;
    return templates.RenderAddSubject(1, admin.FullName(), admin.role, Group::GetGroupName(db, admin.group_id));
}


std::string GroupAPI::AddSubject(const std::unordered_map<std::string, std::string> &data) {
    std::string session = data.find("session")->second;
    Student admin = getStudentBySession(session);
    if (admin.role == Student::Roles::LEADER || admin.role == Student::Roles::ADMIN) {
        try {
            Subject::AddSubject(db, data.find("subject")->second, admin.group_id);
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    } else {
        return "Not enough privileges";
    }
    return "";
}

Student GroupAPI::getStudentBySession(std::string &session) {
    Student st;
    try {
        st = Student::GetStudentBySession(db, session);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return st;
}

std::string GroupAPI::AddDeadline(const std::unordered_map<std::string, std::string> &data) {
    std::string session = data.find("session")->second;
    Student admin = getStudentBySession(session);
    std::string time = data.find("time")->second;
    std::string deadlineName = data.find("name")->second;
    int subjectID = std::stoi(data.find("subject")->second);
    if (admin.role == Student::Roles::LEADER || admin.role == Student::Roles::ADMIN) {
        try {
            Deadline::AddDeadline(db, deadlineName, subjectID, time);
        } catch (std::exception &e) {
            return e.what();
        }
        // Сразу же ставим уведомления за неделю и за сутки до мероприятия для всей группы (на почту)
        std::vector<Student> students = Group::GetMembers(db, admin.group_id);
        struct std::tm tm{};
        std::cout << time << std::endl;
        std::istringstream ss(time);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        time_t schedTime = mktime(&tm);
        for (auto &student : students) {
            std::string name =  deadlineName + " по " + Subject::GetSubjectById(db, subjectID).subject + " " + time;
            SendMail::AddInQueue(db, student.login, name, name, schedTime - WEEK);
            SendMail::AddInQueue(db, student.login, name, name, schedTime - DAY);
        }
    } else {
        return "Недостаточно привилегий";
    }
    return "";
}

std::string GroupAPI::GetDeadlines(const std::unordered_map<std::string, std::string> &data) {
    Student st;
    try {
        st = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }
    std::vector<Deadline> deadlines;
    try {
        deadlines = Deadline::GetDeadlines(db, st.group_id);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }
    json dead;
    dead["deadlines"] = {};
    for (auto &e : deadlines) {
        dead["deadlines"] += {
            {"id", e.id},
            {"name", e.name},
            {"subject", e.subject},
            {"date", e.date_deadline},
        };
    }
    return dead.dump();
}

std::string GroupAPI::GetSubjectsPage(const std::unordered_map<std::string, std::string> &data) {
    Student admin;
    try {
        admin = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }
    return templates.RenderSubjectsList(db, admin.group_id, 1, admin.FullName(), admin.role, Group::GetGroupName(db, admin.group_id));
}

std::string GroupAPI::GetSubjects(const std::unordered_map<std::string, std::string> &data) {
    Student st;
    try {
        st = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }
    std::vector<Subject> subjects;
    try {
        subjects = Subject::ListSubject(db, st.group_id);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }

    json subj;
    subj["subjects"] = {};
    for (auto &e : subjects) {
        subj["subjects"] += {
            {"id", e.id},
            {"name", e.subject},
        };
    }

    return subj.dump();
}

std::string GroupAPI::DeleteDeadline(const std::unordered_map<std::string, std::string> &data) {
    Student st;
    try {
        st = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return e.what();
    }
    try {
        if (st.group_id != Deadline::GetDeadlineById(db, std::stoi(data.find("id")->second)).group_id || st.role != Student::Roles::LEADER) {
            return "Недостаточно привилегий";
        }
    } catch (std::exception &e) {
        return "Дедлайна не существует";
    }
    try {
        Deadline::DeleteDeadline(db, std::stoi(data.find("id")->second));
    } catch (std::exception &e) {
        return e.what();
    }
    return "";
}

std::string GroupAPI::DeleteSubject(const std::unordered_map<std::string, std::string> &data) {
    Student admin;
    try {
        admin = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return e.what();
    }
    Subject s;
    try {
        s = Subject::GetSubjectById(db, std::stoi(data.find("id")->second));
    } catch (std::exception &e) {
        return e.what();
    }
    if (s.group_id != admin.group_id) {
        return "Это не ваш предмет";
    }
    Subject::DeleteSubject(db, s.id);
    return "";
}
