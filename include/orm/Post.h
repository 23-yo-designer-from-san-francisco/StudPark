#ifndef INCLUDE_ORM_POST_H_
#define INCLUDE_ORM_POST_H_

#include <iostream>
#include <vector>
#include <string>
#include "Wrapper.h"


class Post {
 public:
    explicit Post(
            int _id,
            const std::string &head,
            const std::string &body,
            int user_id,
            bool _deleted,
            time_t _created) :
            id(_id),
            post_head(head),
            post_body(body),
            owner(user_id),
            deleted(_deleted),
            created(_created) {}
    static Post GetPost(SQLWrapper &db, int post_id);
    static int AddPost(SQLWrapper &db, const std::string &head, const std::string &body, int user_id, time_t created);
    static void DeletePost(SQLWrapper &db, int id);
    static std::vector<Post> GetPostsByIdGroup(SQLWrapper &db, int id);

    int id;
    std::string post_head;
    std::string post_body;
    int owner;
    bool deleted;
    time_t created;
};

#endif  // INCLUDE_ORM_POST_H_
