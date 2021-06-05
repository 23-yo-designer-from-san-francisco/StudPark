#ifndef INCLUDE_ORM_FILE_H_
#define INCLUDE_ORM_FILE_H_

#include <iostream>
#include <vector>
#include <string>
#include "Wrapper.h"
#include <utility>

class File {
 public:
    explicit File(
            int file_id,
            int owner_id,
            const std::string &file_name,
            const std::string &file_salt) :
            id(file_id),
            owner(owner_id),
            name(file_name),
            salt(file_salt) {}
    static int AddFile(SQLWrapper &db, int owner_id, const std::string &file_name, const std::string &salt);
    static void DeleteFile(SQLWrapper &db, int user_id);
    static File GetFile(SQLWrapper &db, int user_id);
    static std::vector<std::pair<int, std::string>> GetListFiles(SQLWrapper &db, int user_id);

    int id;
    int owner;
    std::string name;
    std::string salt;
};

#endif  // INCLUDE_ORM_FILE_H_
