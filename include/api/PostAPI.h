#ifndef INCLUDE_API_POSTAPI_H_
#define INCLUDE_API_POSTAPI_H_

#include <unordered_map>
#include <string>
#include "API.h"
#include "Comment.h"
#include "Post.h"
#include "Student.h"
#include <map>

class PostAPI : API {
 public:
    PostAPI(SQLWrapper &db) : API(db) {}
    std::string Create(const std::unordered_map<std::string, std::string> &data) override;
    std::string Get(const std::unordered_map<std::string, std::string> &data)    override;
    std::string Update(const std::unordered_map<std::string, std::string> &data) override;
    std::string Delete(const std::unordered_map<std::string, std::string> &data) override;

    std::string Feed(const std::unordered_map<std::string, std::string> &data);
    std::string AddPostComment(const std::unordered_map<std::string, std::string> &data);
    int DeletePostComment(const std::unordered_map<std::string, std::string> &data);
    std::string CreatePostPage(const std::unordered_map<std::string, std::string> &data);
};

#endif  // INCLUDE_API_POSTAPI_H_
