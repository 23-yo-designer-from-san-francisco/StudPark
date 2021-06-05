#include "File.h"
#include "Wrapper.h"
#include "utils_wrapper.hpp"

int File::AddFile(SQLWrapper &db, int owner_id, const std::string &file_name, const std::string &salt) {
    db << "INSERT INTO file (owner, name, salt) values ('"
       << owner_id << "', '"
       << file_name << "', '"
       << salt << "') returning id;";
    db.exec();
    return db.get_int(0);
}

void File::DeleteFile(SQLWrapper &db, int file_id) {
    db << "DELETE FROM file WHERE id = " << file_id << ";";
    db.exec();
}

std::vector<std::pair<int, std::string>> File::GetListFiles(SQLWrapper &db, int user_id) {
    db << "SELECT id, name FROM file WHERE owner = " << user_id << ";";
    db.exec();
    int i = 0;
    std::vector<std::pair<int, std::string>> result;
    while (db.count_tuples() > i) {
        std::pair<int, std::string> request(
                        db.get_int(0, i),
                        db.get_str(1, i));
        result.push_back(request);
        i++;
    }
    return result;
}
File File::GetFile(SQLWrapper &db, int file_id) {
    if (check_existence(db, "file", "id", file_id)) {
        throw std::length_error("ERROR: FIELD file.id NOT FOUND");
    }
    db << "SELECT * FROM file WHERE id = " << file_id << ";";
    db.exec();
    File result(
            db.get_int(0),
            db.get_int(1),
            db.get_str(2),
            db.get_str(3));
    return result;
}
