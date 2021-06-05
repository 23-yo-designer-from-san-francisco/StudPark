#ifndef INCLUDE_TEMPLATES_TEMPLATE_ENGINE_H_
#define INCLUDE_TEMPLATES_TEMPLATE_ENGINE_H_

#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "Post.h"
#include "Student.h"
#include "inja.hpp"
#include "Wrapper.h"
#include "Contact.h"
#include "Student.h"
#include "Group.h"
#include "Subject.h"

using namespace inja;
using json = nlohmann::json;

class TemplateEngine {
 public:
    std::string RenderTemplate(const json& data, const std::string& templateToParse);
    std::string RenderPosts(SQLWrapper &wrapper, int groupId, bool isAuthenticated = false,
                            std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderProfile(SQLWrapper &wrapper, const std::vector<Contact> &contacts,
                              const Student &student, bool isAuthenticated = false,
                              std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderSettings(SQLWrapper &wrapper, const std::vector<Contact> &contacts,
                               const Student &student, bool isAuthenticated = false,
                               std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderErrors(const std::string &error, bool isAuthenticated = false,
                             std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderGroupList(SQLWrapper &wrapper, int groupId, bool isAuthenticated = false,
                                std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderLoginPage(bool isAuthenticated = false, std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderAdminPage(bool isAuthenticated = false, std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderSignupPage(bool isAuthenticated = false, std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderSubjectsList(SQLWrapper &wrapper, int groupId, bool isAuthenticated = false,
                                   std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderAddSubject(bool isAuthenticated = false, std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderCreatePostPage(bool isAuthenticated = false, std::string user = "", int userRole = 0, std::string group = "");
    std::string RenderGroupSuccessfulCreate(const std::string &error, bool isAuthenticated = false,
                             std::string user = "", int userRole = 0, std::string group = "");

 private:
    Environment env;
};

#endif  // INCLUDE_TEMPLATES_TEMPLATE_ENGINE_H_
