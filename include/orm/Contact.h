#ifndef INCLUDE_ORM_CONTACT_H_
#define INCLUDE_ORM_CONTACT_H_

#include <iostream>
#include <vector>
#include <string>
#include "Wrapper.h"

class Contact {
 public:
    enum STEALTH {
        YES = 1,
        NO = 0
    };
    enum CONTACT_TYPE {
        VK = 0,
        TELEGRAM = 1,
        INSTAGRAM = 2,
        GIT = 3,
        TIKTOK = 4,
        DISCORD = 5,
        SKYPE = 6,
        PHONE = 7,
        EMAIL = 8
    };
    explicit Contact(
            int _id,
            int user_stealth,
            int user_type_contacts,
            const std::string &user_url,
            int user_id) :
            id(_id),
            stealth(user_stealth),
            user(user_id),
            type_contacts(user_type_contacts),
            url(user_url) {}
    static std::vector<Contact> GetContacts(SQLWrapper &db, int user_id);
    static int AddContact(SQLWrapper &db, int contact_type, const std::string &user_url, int stealth, int user_id);
    static void DeleteContacts(SQLWrapper &db, int id);
    static std::string GetNameContact(int contact_type);

    int id;
    int stealth;
    int type_contacts;
    std::string url;
    int user;
};

#endif  // INCLUDE_ORM_CONTACT_H_
