#include "Share.h"
#include "Wrapper.h"
#include "utils_wrapper.hpp"

int Share::AddShare(SQLWrapper &db, int recipient_id, int owner_id, int file_id) {
    db << "INSERT INTO share (recipient, owner, file_fk) values ('"
       << recipient_id << "', "
       << owner_id << ", "
       << file_id <<  ") returning id;";
    db.exec();

    return db.get_int(0);
}

void Share::DeleteShare(SQLWrapper &db, int id) {
    db << "DELETE FROM share WHERE id = " << id << ";";
    db.exec();
}

Share Share::GetList(SQLWrapper &db, int owner_id) {
    if (check_existence(db, "share", "owner", owner_id)) {
        throw std::length_error("ERROR: FIELD share.owner NOT FOUND");
    }
    db << "SELECT * FROM share WHERE owner = " << owner_id << ";";
    db.exec();
    Share result(
                   db.get_int(0),
                   db.get_int(1),
                   db.get_int(2),
                   db.get_int(3));
    return result;
}

std::vector<Share> Share::GetFilesForId(SQLWrapper &db, int owner_id, int recipient_id) {
    db << "SELECT * FROM share WHERE owner = " << owner_id << " AND recipient = " << recipient_id << ";";
    db.exec();
    int i = 0;
    std::vector<Share> result;
    while (db.count_tuples() > i) {
        Share request(
                db.get_int(0, i),
                db.get_int(1, i),
                db.get_int(2, i),
                db.get_int(3, i));
        result.push_back(request);
        i++;
    }
    return result;
}
