#include <iostream>
#include "Wrapper.h"
#include "AttachmentPost.h"
#include "utils_wrapper.hpp"

int AttachmentPost::AddAttachmentPost(SQLWrapper &db, int post_id, const std::string &file_name,
        const std::string &path, int type) {
    db << "INSERT INTO attachment_post (post_id, name, path, type) values ('"
       << post_id << "', '"
       << file_name << "', '"
       << path << "', "
       << type << ") returning id;";
    db.exec();

    return db.get_int(0);
}

std::vector<AttachmentPost> AttachmentPost::GetAttachmentPost(SQLWrapper &db, int post_id) {
    db << "SELECT * FROM attachment_post WHERE post_id = " << post_id << " AND deleted = false;";
    db.exec();
    int i = 0;
    std::vector<AttachmentPost> result;
    while (db.count_tuples() > i) {
        AttachmentPost request(
                db.get_int(0, i),
                db.get_int(1, i),
                db.get_int(2, i),
                db.get_str(3, i),
                db.get_str(4, i));
        result.push_back(request);
        i++;
    }
    return result;
}

void AttachmentPost::DeleteAttachmentPost(SQLWrapper &db, int id) {
    if (check_existence(db, "attachment_post", "id", id)) {
        throw std::length_error("ERROR: FIELD attachment_post.id NOT FOUND");
    }
    db << "UPDATE attachment_post SET deleted = true WHERE id = " << id << ";";
    db.exec();
}
