#ifndef INCLUDE_ORM_ATTACHMENTPOST_H_
#define INCLUDE_ORM_ATTACHMENTPOST_H_

#include <iostream>
#include <vector>
#include <string>
#include "Wrapper.h"

class AttachmentPost {
 public:
    enum TYPE_ATTACHMENT {
        PHOTO = 0,
        VIDEO = 1,
        GIF = 2
    };
    explicit AttachmentPost(
            int _id,
            int _post_id,
            int _type = -1,
            const std::string &file_name = "",
            const std::string &_path = "") :
            id(_id),
            post_id(_post_id),
            type(_type),
            name(file_name),
            path(_path) {}
    static std::vector<AttachmentPost> GetAttachmentPost(SQLWrapper &db, int post_id);
    static int AddAttachmentPost(SQLWrapper &db, int post_id, const std::string &file_name,
            const std::string &path, int type = AttachmentPost::PHOTO);
    static void DeleteAttachmentPost(SQLWrapper &db, int id);

    int id;
    int post_id;
    int type;
    std::string name;
    std::string path;
};

#endif  // INCLUDE_ORM_ATTACHMENTPOST_H_
