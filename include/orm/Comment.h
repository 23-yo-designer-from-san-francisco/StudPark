#ifndef INCLUDE_ORM_COMMENT_H_
#define INCLUDE_ORM_COMMENT_H_

#include <ctime>
#include <string>
#include <vector>
#include "Wrapper.h"
#include "Student.h"

class Comment {
 public:
    explicit Comment(
            int comment_id,
            int post,
            int author,
            const std::string &body_comment,
            time_t _created) :
            id(comment_id),
            post_id(post),
            author_id(author),
            text(body_comment),
            created(_created) {}
    static std::vector<Comment> GetComment(SQLWrapper &db, int post_id);
    static int AddComment(SQLWrapper &db, int post_id, int author_id, const std::string &body_comment);
    static void DeleteComment(SQLWrapper &db, int comment_id);

    int id;
    int post_id;
    int author_id;
    std::string text;
    time_t created;
};

#endif  // INCLUDE_ORM_COMMENT_H_
