#ifndef INCLUDE_ORM_SHARE_H_
#define INCLUDE_ORM_SHARE_H_

#include <string>
#include "Wrapper.h"
#include <vector>

class Share {
 public:
    explicit Share(
            int share_id,
            int recipient_id,
            int owner_id,
            int file_id) :
            id(share_id),
            recipient(recipient_id),
            owner(owner_id),
            file_fk(file_id) {}
    static int AddShare(SQLWrapper &db, int recipient_id, int owner_id, int file_id);
    static void DeleteShare(SQLWrapper &db, int id);
    static Share GetList(SQLWrapper &db, int owner_id);
    static std::vector<Share> GetFilesForId(SQLWrapper &db, int owner_id, int recipient_id);

    int id;
    int recipient;
    int owner;
    int file_fk;
};

#endif  // INCLUDE_ORM_SHARE_H_
