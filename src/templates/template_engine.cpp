#include "template_engine.h"

std::string TemplateEngine::RenderTemplate(const json& data, const std::string& templateToParse) {
    env.set_expression("{{", "}}");
    env.set_statement("{%", "%}");
    Template temp = env.parse_template("templates/" + templateToParse);
    env.write(temp, data, "created_templates/" + templateToParse);
    std::string result = env.render(temp, data);
    return result;
}

std::string TemplateEngine::RenderProfile(SQLWrapper &wrapper, const std::vector<Contact> &contacts,
                                          const Student& student, bool isAuthenticated, std::string user,
                                          int userRole, std::string group) {
    json data {
            {"isAuthenticated", isAuthenticated},
            {"user", user},
            {"role", userRole},
            {"group", group},
            {"student", student.second_name + " " + student.first_name + " " + student.patronymic},
            {"information", {
                {
                    {"label", "ФИО"},
                    {"data", student.second_name + " " + student.first_name + " " + student.patronymic}
                },
                {
                    {"label", "Группа"},
                    {"data", Group::GetGroupName(wrapper, student.group_id)},
                },
                {
                    {"label", "День рождения"},
                    {"data", student.birthday}
                },
                {
                    {"label", "Email"},
                    {"data", student.login},
                },
            }}
    };
    for (auto &contact : contacts) {
        data["information"] += {
                {"label", Contact::GetNameContact(contact.type_contacts)},
                {"data", "<a href=" + contact.url + ">" + contact.url + "</a>"}
        };
    }
    return RenderTemplate(data, "profile.html");
}

std::string TemplateEngine::RenderSettings(SQLWrapper &wrapper, const std::vector<Contact> &contacts,
                                           const Student &student, bool isAuthenticated, std::string user,
                                           int userRole, std::string group) {
    json data {
            {"isAuthenticated", isAuthenticated},
            {"user", user},
            {"role", userRole},
            {"group", group},
            {"FIO", student.second_name + " " + student.first_name + " " + student.patronymic},
            {"birthday", student.birthday},
            {"notification", student.notification},
            {"contacts", {
            }}
    };
    std::vector<int> filledContacts;
    for (auto &contact : contacts) {
        data["contacts"] += {
                {"id", std::to_string(contact.type_contacts)},
                {"label", Contact::GetNameContact(contact.type_contacts)},
                {"data", contact.url}
        };
        filledContacts.push_back(contact.type_contacts);
    }
    int contactsAmount = 8;
    for (int index = 0; index < contactsAmount; ++index) {
        if (std::find(filledContacts.begin(), filledContacts.end(), index) == filledContacts.end()) {
            data["contacts"] += {
                    {"id", std::to_string(index)},
                    {"label", Contact::GetNameContact(index)},
                    {"data", ""}
            };
        }
    }
    return RenderTemplate(data, "settings.html");
}

std::string TemplateEngine::RenderGroupList(SQLWrapper &wrapper, int groupId, bool isAuthenticated, std::string user,
                                            int userRole, std::string group) {
    std::vector<Student> students = Group::GetMembers(wrapper, groupId);
    json data {
        {"isAuthenticated", isAuthenticated},
        {"user", user},
        {"role", userRole},
        {"group", group},
        {"group_name", Group::GetGroupName(wrapper, groupId)},
        {"students",
            {}
        }
    };
    int index = 1;
    for (auto &student : students) {
        std::vector<Contact> studentContacts = Contact::GetContacts(wrapper, student.id);
        std::string phoneNumber;
        std::string email = student.login;
        for (auto &contact : studentContacts) {
            if (contact.type_contacts == 7)
                phoneNumber = contact.url;
        }
        data["students"] += {
                {"index", index},
                {"FIO", student.second_name + " " + student.first_name + " " + student.patronymic},
                {"birthday", student.birthday},
                {"phone", phoneNumber},
                {"email", email}
        };
        index += 1;
    }
    return RenderTemplate(data, "group_list.html");
}

std::string TemplateEngine::RenderErrors(const std::string &error, bool isAuthenticated, std::string user, int userRole,
                                         std::string group) {
    json data {
        {"isAuthenticated", isAuthenticated},
        {"user", user},
        {"role", userRole},
        {"group", group},
        {"error", error}
    };
    return RenderTemplate(data, "errors.html");
}

std::string TemplateEngine::RenderLoginPage(bool isAuthenticated, std::string user, int userRole, std::string group) {
    json data{
        {"isAuthenticated", isAuthenticated},
        {"user", user},
        {"role", userRole},
        {"group", group},
    };
    return RenderTemplate(data, "login.html");
}

std::string TemplateEngine::RenderAdminPage(bool isAuthenticated, std::string user, int userRole, std::string group) {
    json data{
        {"isAuthenticated", isAuthenticated},
        {"user", user},
        {"role", userRole},
        {"group", group},
    };
    return RenderTemplate(data, "admin_panel.html");
}

std::string TemplateEngine::RenderSignupPage(bool isAuthenticated, std::string user, int userRole, std::string group) {
    json data{
        {"isAuthenticated", isAuthenticated},
        {"user", user},
        {"role", userRole},
        {"group", group},
    };
    return RenderTemplate(data, "registration.html");
}

std::string TemplateEngine::RenderPosts(SQLWrapper &wrapper, int groupId, bool isAuthenticated,
                                        std::string user, int userRole, std::string group) {
    json data {
            {"isAuthenticated", isAuthenticated},
            {"user", user},
            {"role", userRole},
            {"group", group},
            {"group_name", Group::GetGroupName(wrapper, groupId)},
            {"posts",
                {}
            }
    };
    std::vector<Post> posts = Post::GetPostsByIdGroup(wrapper, groupId);
    for (const auto& post : posts) {
        std::string author = (Student::GetStudentById(wrapper, post.owner).second_name + " " +
                Student::GetStudentById(wrapper, post.owner).first_name);
        char buffer[256];
        strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", localtime(&post.created));
        std::string str(buffer);
        data["posts"] += {
            {"id", post.id},
            {"title", post.post_head},
            {"text", post.post_body},
            {"author", author},
            {"datetime", str},
        };
    }
    return RenderTemplate(data, "main.html");
}


std::string TemplateEngine::RenderSubjectsList(SQLWrapper &wrapper, int groupId, bool isAuthenticated, std::string user,
                                               int userRole, std::string group) {
    json data {
        {"isAuthenticated", isAuthenticated},
        {"user", user},
        {"role", userRole},
        {"group", group},
        {"group_name", Group::GetGroupName(wrapper, groupId)},
        {"subjects",
            {}
        }
    };
    int index = 1;
    std::vector<Subject> subjects = Subject::ListSubject(wrapper, groupId);
    for (auto &subject : subjects) {
        data["subjects"] += {
            {"id", subject.id},
            {"index", index},
            {"name", subject.subject},
        };
        index += 1;
    }
    return RenderTemplate(data, "subjects_list.html");
}

std::string TemplateEngine::RenderAddSubject(bool isAuthenticated, std::string user, int userRole, std::string group) {
    json data {
        {"isAuthenticated", isAuthenticated},
        {"user", user},
        {"role", userRole},
        {"group", group},
    };
    return RenderTemplate(data, "add_subject.html");
}

std::string TemplateEngine::RenderCreatePostPage(bool isAuthenticated, std::string user, int userRole, std::string group) {
    json data {
        {"isAuthenticated", isAuthenticated},
        {"user", user},
        {"role", userRole},
        {"group", group},
    };
    return RenderTemplate(data, "create_post.html");
}

std::string TemplateEngine::RenderGroupSuccessfulCreate(const std::string &error, bool isAuthenticated, std::string user, int userRole,
                                         std::string group) {
    json data {
        {"isAuthenticated", isAuthenticated},
        {"user", user},
        {"role", userRole},
        {"group", group},
        {"error", error}
    };
    return RenderTemplate(data, "successful_group_create.html");
}
