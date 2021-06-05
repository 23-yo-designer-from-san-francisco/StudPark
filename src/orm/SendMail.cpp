#include <iostream>
#include "SendMail.h"
#include "Wrapper.h"
#include "utils_wrapper.hpp"

std::vector<SendMail> SendMail::NeedSend(SQLWrapper &db) {
    db << "SELECT * FROM send_mail WHERE time_send <= now();";
    db.exec();
    int i = 0;
    std::vector<SendMail> result;
    while (db.count_tuples() > i) {
        SendMail request(
                db.get_int(0, i),
                db.get_str(1, i),
                db.get_str(2, i),
                db.get_str(3, i),
                db.get_time_t(4, i));
        result.push_back(request);
        i++;
    }
    return result;
}

int SendMail::AddInQueue(SQLWrapper &db, const std::string &recipient, const std::string &head_mail,
                         const std::string &body_mail, time_t time_send) {
    db << "INSERT INTO send_mail (recipient, head_mail, body_mail, time_send) values ('"
       << recipient << "', '"
       << head_mail << "', '"
       << body_mail << "', to_timestamp("<< time_send << ")) returning id;";
    db.exec();

    return db.get_int(0);
}

void SendMail::DeleteFromQueue(SQLWrapper &db, int id) {
    db << "DELETE FROM send_mail WHERE id = " << id << ";";
    db.exec();
}
