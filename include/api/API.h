#pragma once

#include "Wrapper.h"
#include "template_engine.h"
#include <string>
#include <unordered_map>

#include <map>

class API {
 public:
    API(SQLWrapper &db) : db(db) {}
    ~API() = default;
    virtual std::string Create(const std::unordered_map<std::string, std::string> &data) = 0;
    virtual std::string Get(const std::unordered_map<std::string, std::string> &data)    = 0;
    virtual std::string Update(const std::unordered_map<std::string, std::string> &data) = 0;
    virtual std::string Delete(const std::unordered_map<std::string, std::string> &data) = 0;
    TemplateEngine templates;
    SQLWrapper &db;
};
