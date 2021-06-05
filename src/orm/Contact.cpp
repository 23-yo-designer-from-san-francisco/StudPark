#include "Contact.h"
#include "Wrapper.h"
#include "utils_wrapper.hpp"

int Contact::AddContact(SQLWrapper &db, const int contact_type, const std::string &user_url, const int stealth, const int user_id) {
    db << "INSERT INTO contacts (stealth, type, url, user_id) values ("
       << stealth << ", "
       << contact_type << ", '"
       << user_url << "', "
       << user_id << ") returning id;";
    db.exec();
    return db.get_int(0);
}

std::vector<Contact> Contact::GetContacts(SQLWrapper &db, int user_id) {
    db << "SELECT * FROM contacts WHERE user_id = " << user_id << ";";
    db.exec();
    int i = 0;
    std::vector<Contact> result;
    while (db.count_tuples() > i) {
        Contact request(
                db.get_int(0, i),
                db.get_int(1, i),
                db.get_int(2, i),
                db.get_str(3, i),
                db.get_int(4, i));
        result.push_back(request);
        i++;
    }
    return result;
}

void Contact::DeleteContacts(SQLWrapper &db, int id) {
    db << "DELETE FROM contacts WHERE id = " << id << ";";
    db.exec();
}

std::string Contact::GetNameContact(const int contact_type) {
    switch (contact_type) {
        case 0:
            return "VK";
        case 1:
            return "Telegram";
        case 2:
            return "Instagram";
        case 3:
            return "Git";
        case 4:
            return "TikTok";
        case 5:
            return "Discord";
        case 6:
            return "Skype";
        case 7:
            return "Номер телефона";
        case 8:
            return "Email";
        default:
            return "Another";
    }
}
