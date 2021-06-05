#include "UserAPI.h"
#include "Student.h"
#include "Group.h"

std::string UserAPI::Login() {
    return templates.RenderLoginPage();
}

std::string UserAPI::Create(const std::unordered_map<std::string, std::string> &data) {
    std::string password = data.find("password")->second;
    std::string password_enc = sha256(password);
    std::string email = data.find("user_login")->second;
    if (!emailIsValid(email)) {
        std::cout << "Email is invalid" << std::endl;
        return templates.RenderErrors("Формат Email неверный");
    }
  //  if (!passwordMeetsRequirements(password)) {
  //    std::cout << "Password does not meet the requirements" << std::endl;
  //    // Render template `error(wrong_password)`
  //    return "";
  //  }
    int groupID;
    try {
        groupID = Group::GetGroupByJoinCode(db, data.find("join_code")->second);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return templates.RenderErrors("Неверный код доступа");
    }

    int userID = Student::AddStudentRegistration(db, data.find("f_name")->second, data.find("s_name")->second,
                              data.find("user_patronymic")->second, email,
                              password_enc, groupID, data.find("birthday")->second, Student::Roles::STUDENT);
    return "";
}

std::string UserAPI::Get(const std::unordered_map<std::string, std::string> &data) {
    return "";  // Render "User profile" page
}
std::string UserAPI::Update(const std::unordered_map<std::string, std::string> &data) {
    Student st;
    try {
        st = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }
    std::unordered_map<std::string, std::string> contacts = data;
    contacts.erase("session");

    std::unordered_map<std::string, std::string> updateData;

    for (auto &contact : contacts) {
        if (!contact.second.empty()) {
            try {
                int type = std::stoi(contact.first);
                Contact::AddContact(db, type, contact.second, 0, st.id);
            } catch (...) {
                updateData[contact.first] = contact.second;
            }
        }
    }
    if (!updateData.empty()) {
        // Student::UpdateStudent(db, updateData);
    }
    return "";  // AJAX
}
std::string UserAPI::Delete(const std::unordered_map<std::string, std::string> &data) {
    return "";  // Only for admin, AJAX
}

std::pair<std::string, std::string> UserAPI::Authenticate(const std::unordered_map<std::string, std::string> &data) {
    int student = 0;
    std::cout << data.find("login")->second << " " << data.find("password")->second << std::endl;
    try {
        student = Student::GetIdByLoginPassword(db, data.find("login")->second, sha256(data.find("password")->second));
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    if (student != 0) {
        std::string token = randomString(64);
        int session = Session::AddSession(db, student, token, time(nullptr));
        return {token, ""};
    } else {
        return {"", templates.RenderErrors("Проверьте правильность ввода логина и пароля")};
    }
}

std::string UserAPI::Authorize(const std::unordered_map<std::string, std::string> &data) {
    Student st = Student::GetStudentBySession(db, data.find("session")->second);
    return st.first_name;
}

std::string UserAPI::Logout(const std::unordered_map<std::string, std::string> &data) {
    std::string session = data.find("session")->second;
    try {
        Student::GetStudentBySession(db, session);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }

    Session::DeleteSession(db, session);

    return "";  // Render page login()
}

int UserAPI::AddContact(const std::unordered_map<std::string, std::string> &data) {
    Student st = Student::GetStudentBySession(db, data.find("session")->second);
    try {
        Contact::AddContact(db, std::stoi(data.find("type")->second),
                            data.find("url")->second,
                            std::stoi(data.find("stealth")->second), st.id);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 200;  // AJAX
}

bool UserAPI::emailIsValid(std::string &email) {
    std::regex regexEmail("(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|\"(?:[\x01-\x08\x0b\x0c\x0e-\x1f"
                "\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*\")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+"
                "[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4]"
                "[0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-"
                "\x09\x0b\x0c\x0e-\x7f])+)\\])");

    return std::regex_match(email, regexEmail);
}

bool UserAPI::passwordMeetsRequirements(std::string &password) {
    std::regex regexPassword("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$");
    return std::regex_match(password, regexPassword);
}

std::string UserAPI::SignupPage() {
    return templates.RenderSignupPage();
}

std::string UserAPI::Profile(const std::unordered_map<std::string, std::string> &data) {
    Student admin;
    try {
        admin = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        templates.RenderErrors(e.what());
    }
    return templates.RenderProfile(db, Contact::GetContacts(db, admin.id), admin, 1, admin.FullName(), admin.role, Group::GetGroupName(db, admin.group_id));
}

std::string UserAPI::SettingsPage(const std::unordered_map<std::string, std::string> &data) {
    Student st;
    try {
        st = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        templates.RenderErrors(e.what());
    }
    std::vector<Contact> contacts = Contact::GetContacts(db, st.id);
    return templates.RenderSettings(db, contacts, st, 1, st.FullName(), st.role, Group::GetGroupName(db, st.group_id));
}
