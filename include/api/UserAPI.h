#ifndef INCLUDE_API_USERAPI_H_
#define INCLUDE_API_USERAPI_H_

#include <utility>
#include <unordered_map>
#include <map>
#include <string>
#include <regex>
#include "API.h"
#include "openssl/sha.h"
#include "Utils.h"
#include "Session.h"
#include "Contact.h"

class UserAPI : API {
 public:
    UserAPI(SQLWrapper &db) : API(db) {}
    std::string Create(const std::unordered_map<std::string, std::string> &data) override;
    std::string Get(const std::unordered_map<std::string, std::string> &data)    override;
    std::string Update(const std::unordered_map<std::string, std::string> &data) override;
    std::string Delete(const std::unordered_map<std::string, std::string> &data) override;
    std::pair<std::string, std::string> Authenticate(const std::unordered_map<std::string, std::string> &data);
    std::string Authorize(const std::unordered_map<std::string, std::string> &data);
    std::string Login();
    std::string Logout(const std::unordered_map<std::string, std::string> &data);
    int AddContact(const std::unordered_map<std::string, std::string> &data);
    std::string SignupPage();
    std::string Profile(const std::unordered_map<std::string, std::string> &data);
    std::string SettingsPage(const std::unordered_map<std::string, std::string> &data);
 private:
    bool emailIsValid(std::string &email);
    bool passwordMeetsRequirements(std::string &password);
};

#endif  // INCLUDE_API_USERAPI_H_
