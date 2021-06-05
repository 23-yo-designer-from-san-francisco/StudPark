#ifndef INCLUDE_API_GROUPAPI_H_
#define INCLUDE_API_GROUPAPI_H_

#include <map>
#include <string>
#include <unordered_map>
#include "API.h"
#include "Group.h"
#include "Student.h"
#include "Subject.h"
#include "Wrapper.h"
#include "SendMail.h"
#include <boost/locale.hpp>

using json = nlohmann::json;

#define WEEK 604800
#define DAY  86400

class GroupAPI : API {
 public:
    GroupAPI(SQLWrapper &db) : API(db) {}
    std::string Create(const std::unordered_map<std::string, std::string> &data) override;
    std::string Get(const std::unordered_map<std::string, std::string> &data) override;
    std::string Update(const std::unordered_map<std::string, std::string> &data) override;
    std::string Delete(const std::unordered_map<std::string, std::string> &data) override;

    std::string ExportGroupList(const std::unordered_map<std::string, std::string> &data);
    std::string AddSubject(const std::unordered_map<std::string, std::string> &data);
    std::string AddSubjectPage(const std::unordered_map<std::string, std::string> &data);
    std::string AddDeadline(const std::unordered_map<std::string, std::string> &data);
    std::string CreatePage(const std::unordered_map<std::string, std::string> &data);
    std::string GetDeadlines(const std::unordered_map<std::string, std::string> &data);
    std::string DeleteDeadline(const std::unordered_map<std::string, std::string> &data);
    std::string GetSubjects(const std::unordered_map<std::string, std::string> &data);
    std::string GetSubjectsPage(const std::unordered_map<std::string, std::string> &data);
    std::string DeleteSubject(const std::unordered_map<std::string, std::string> &data);
 private:
    Student getStudentBySession(std::string &session);
};

#endif  // INCLUDE_API_GROUPAPI_H_
