#include "Wrapper.h"
#include "Comment.h"
#include "utils_wrapper.hpp"

int Comment::AddComment(SQLWrapper &db, int post_id, int author_id, const std::string &body_comment) {
    if (check_existence(db, "post", "id", post_id)) {
        throw std::length_error("ERROR: FIELD post.id NOT FOUND ");
    }
    if (check_existence(db, "student", "id", author_id)) {
        throw std::length_error("ERROR: FIELD student.id student.NOT FOUND ");
    }

    db << "INSERT INTO comment (post_id, author_id, text, created) values ('"
       << post_id << "', "
       << author_id << ", '"
       << body_comment << "', "
       << "to_timestamp(" << time(nullptr) << ")) returning id;";
    db.exec();

    return db.get_int(0);
}

void Comment::DeleteComment(SQLWrapper &db, int comment_id) {
    if (check_existence(db, "comment", "id", comment_id)) {
        throw std::length_error("ERROR: FIELD comment.id NOT FOUND");
    }

    db << "UPDATE comment SET deleted = true WHERE id = " << comment_id << ";";
    db.exec();
}

std::vector<Comment> Comment::GetComment(SQLWrapper &db, int post_id) {
    db << "SELECT * FROM comment WHERE post_id = " << post_id << " AND deleted = false;";
    db.exec();
    int i = 0;
    std::vector<Comment> result;
    while (db.count_tuples() > i) {
        Comment request(
                db.get_int(0, i),
                db.get_int(1, i),
                db.get_int(2, i),
                db.get_str(3, i),
                db.get_time_t(4, i));
        result.push_back(request);
        i++;
    }
    return result;
}
